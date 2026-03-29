#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>
#include <functional>

template <typename RandomIt, typename Compare>
void order(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
    {
        return;
    }
    for (auto i = std::next(first); i != last; ++i)
    {
        for (auto j = i; j != first && comp(*j, *std::prev(j)); --j)
        {
            std::iter_swap(j, std::prev(j));
        }
    }
}

template <typename RandomIt, typename Compare>
RandomIt quick_partition(RandomIt first, RandomIt last, Compare comp)
{
    auto pivot_it = std::prev(last);
    auto pivot_value = *pivot_it;
    auto i = first;
    for (auto j = first; j != pivot_it; ++j)
    {
        if (!comp(pivot_value, *j))
        {
            std::iter_swap(i, j);
            std::advance(i, 1);
        }
    }
    std::iter_swap(i, pivot_it);
    return i;
}

template <typename RandomIt, typename Compare>
void quick_split(RandomIt first, RandomIt last, Compare comp)
{
    auto dist = std::distance(first, last);
    if (dist <= 64)
    {
        order(first, last, comp);
        return;
    }
    RandomIt pivot_it = quick_partition(first, last, comp);
    quick_split(first, pivot_it, comp);
    quick_split(std::next(pivot_it), last, comp);
}

template <typename Container, typename Compare = std::less<>>
void sort(Container& container, Compare comp = Compare{})
{
    quick_split(std::begin(container), std::end(container), comp);
}

bool greater_custom(int a, int b) {
    return a > b;
}

TEST(SortTest, StandardLess)
{
    std::vector<int> data = {5, 2, 9, 1, 5, 6};
    sort(data, std::less<int>());
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

TEST(SortTest, CustomFreeFunction)
{
    std::vector<int> data = {1, 2, 3, 4, 5};
    sort(data, greater_custom);
    EXPECT_TRUE(std::ranges::is_sorted(data, std::greater<int>()));
}

TEST(SortTest, LambdaComparator)
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto paritySort = [](int a, int b) {
        if ((a % 2 == 0) != (b % 2 == 0))
            return (a % 2 == 0);
        return a < b;
    };
    sort(data, paritySort);

    std::vector<int> expected = {2, 4, 6, 1, 3, 5};
    EXPECT_EQ(data, expected);
}

TEST(SortTest, StringSorting)
{
    std::vector<std::string> data = {"banana", "apple", "cherry"};
    sort(data);
    EXPECT_EQ(data[0], "apple");
    EXPECT_EQ(data[2], "cherry");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}