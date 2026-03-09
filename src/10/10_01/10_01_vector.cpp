#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <cstdint>

// GCC libstdc++ vector uses expansion factor 2

template <typename T> class VectorMemoryTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
        vec.clear();
        vec.shrink_to_fit();
    }

    std::vector<T> vec;
};

using VectorTypes = ::testing::Types<int, double, std::array<char, 120>>;

TYPED_TEST_SUITE(VectorMemoryTest, VectorTypes);

TYPED_TEST(VectorMemoryTest, GrowthFactorIsDouble)
{
    this->vec.push_back(TypeParam{});
    size_t current_capacity = this->vec.capacity();

    int expansions_found = 0;
    while (expansions_found < 5)
    {
        if (this->vec.size() == current_capacity)
        {
            size_t old_capacity = current_capacity;
            this->vec.push_back(TypeParam{});
            current_capacity = this->vec.capacity();

            EXPECT_EQ(current_capacity, old_capacity * 2);
            expansions_found++;
        }
        else
        {
            this->vec.push_back(TypeParam{});
        }
    }
}

TYPED_TEST(VectorMemoryTest, RelocationOnExpansion)
{
    this->vec.reserve(10);
    this->vec.push_back(TypeParam{});

    TypeParam* initial_addr = &(this->vec[0]);

    while (this->vec.size() < this->vec.capacity())
    {
        this->vec.push_back(TypeParam{});
    }

    EXPECT_EQ(initial_addr, &(this->vec[0]));

    this->vec.push_back(TypeParam{});

    EXPECT_NE(initial_addr, &(this->vec[0]));
}

TYPED_TEST(VectorMemoryTest, ReservePreventsFrequentAllocations)
{
    const size_t target_size = 1000;
    this->vec.reserve(target_size);

    size_t initial_capacity = this->vec.capacity();
    EXPECT_GE(initial_capacity, target_size);

    TypeParam* initial_addr = nullptr;
    for (size_t i = 0; i < target_size; ++i)
    {
        this->vec.push_back(TypeParam{});

        if (i == 0)
        {
            initial_addr = &(this->vec[0]);
        }

        ASSERT_EQ(initial_addr, &(this->vec[0]));
        ASSERT_EQ(initial_capacity, this->vec.capacity());
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}