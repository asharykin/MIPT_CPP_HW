#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>

template <typename RandomIt>
void order(RandomIt first, RandomIt last)
{
    if (first == last) return;

    for (auto i = std::next(first); i != last; ++i)
    {
        for (auto j = i; j != first && *std::prev(j) > *j; --j)
        {
            std::iter_swap(j, std::prev(j));
        }
    }
}

template <typename RandomIt>
RandomIt partition(RandomIt first, RandomIt last)
{
    auto pivot_it = std::prev(last);
    auto pivot_value = *pivot_it;

    auto i = first;
    for (auto j = first; j != pivot_it; ++j)
    {
        if (*j <= pivot_value)
        {
            std::iter_swap(i, j);
            std::advance(i, 1);
        }
    }
    std::iter_swap(i, pivot_it);
    return i;
}

template <typename RandomIt>
void quick_split(RandomIt first, RandomIt last)
{
    auto dist = std::distance(first, last);

    if (dist <= 64)
    {
        order(first, last);
        return;
    }

    RandomIt pivot_it = partition(first, last);

    quick_split(first, pivot_it);
    quick_split(std::next(pivot_it), last);
}

template <typename Container>
void sort(Container& container)
{
    quick_split(std::begin(container), std::end(container));
}

TEST(SortTest, EmptyVector)
{
    std::vector<int> data;
    sort(data);
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

TEST(SortTest, SingleElement)
{
    std::vector<int> data = {42};
    sort(data);
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

TEST(SortTest, Duplicates)
{
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sort(data);
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

TEST(SortTest, StandardArray)
{
    int data[] = {10, -1, 3, 0, 5};
    sort(data);
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

TEST(SortTest, ReverseSortedLargeVector)
{
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 0);
    std::ranges::reverse(data);

    sort(data);
    EXPECT_TRUE(std::ranges::is_sorted(data));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}