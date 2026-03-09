#include <gtest/gtest.h>
#include <deque>
#include <array>
#include <cstdint>

// GCC libstdc++ deque uses 512-byte pages
// Elements per page = max(512 / sizeof(T), 1

template<typename T>
class DequeMemoryTest : public ::testing::Test
{
protected:

    static constexpr size_t GCC_DEQUE_PAGE_SIZE = 512;

    static constexpr size_t expect_elements_per_page()
    {
        return sizeof(T) < GCC_DEQUE_PAGE_SIZE ? (GCC_DEQUE_PAGE_SIZE / sizeof(T)) : 1;
    }

    void SetUp() override
    {
        deque.clear();
    }

    std::deque<T> deque;
};

using DequeTypes = ::testing::Types<int, double, std::array<char, 120>, std::array<char, 600> >;

TYPED_TEST_SUITE(DequeMemoryTest, DequeTypes);

TYPED_TEST(DequeMemoryTest, MatchesGccSpecificPageSize) {
    size_t page_size = this->expect_elements_per_page();
    const size_t num_pages = 3;

    size_t elems_per_page = 0;

    this->deque.push_back(TypeParam{});

    for (size_t i = 1; i < page_size * num_pages; ++i) {
        this->deque.push_back(TypeParam{});

        if (&(this->deque[i]) != &(this->deque[i - 1]) + 1) {
            elems_per_page = i;
            break;
        }
    }

    EXPECT_EQ(elems_per_page, page_size);
}

TYPED_TEST(DequeMemoryTest, ValidatesContiguityAcrossMultiplePages) {
    size_t page_size = this->expect_elements_per_page();
    const size_t num_pages = 3;

    for (size_t i = 0; i < page_size * num_pages; ++i) {
        this->deque.push_back(TypeParam{});
    }
    
    for (size_t p = 1; p < num_pages; ++p) {
        size_t boundary_idx = p * page_size;

        if (page_size > 1) {
            EXPECT_EQ(&(this->deque[boundary_idx - 1]), &(this->deque[boundary_idx - 2]) + 1);
        }

        EXPECT_NE(&(this->deque[boundary_idx]), &(this->deque[boundary_idx - 1]) + 1);
    }
}

TYPED_TEST(DequeMemoryTest, PushFrontStability) {
    this->deque.push_back(TypeParam{});
    TypeParam* addr_before = &(this->deque.back());

    size_t page_size = this->expect_elements_per_page();
    for (size_t i = 0; i < page_size * 10; ++i) {
        this->deque.push_front(TypeParam{});
    }

    TypeParam* addr_after = &(this->deque.back());
    EXPECT_EQ(addr_before, addr_after);
}

TYPED_TEST(DequeMemoryTest, PushBackStability) {
    size_t page_size = this->expect_elements_per_page();
    for (size_t i = 0; i < page_size; ++i) {
        this->deque.push_back(TypeParam{});
    }

    TypeParam* first_page_addr = &(this->deque[0]);

    for (size_t i = 0; i < page_size * 50; ++i) {
        this->deque.push_back(TypeParam{});
    }

    EXPECT_EQ(first_page_addr, &(this->deque[0]));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
