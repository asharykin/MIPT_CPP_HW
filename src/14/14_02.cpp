#include <algorithm>
#include <functional>
#include <future>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>
#include <gtest/gtest.h>

///////////////////////////////////////////////////////////////////////////

template < std::ranges::view V, typename T, typename Op >
auto fold(V view, T init, Op op) -> T
{
    auto begin = std::begin(view), end = std::end(view);

    if (auto size = std::distance(begin, end), half = size / 2; size > 16)
    {
        std::ranges::subrange range(begin, std::next(begin, half));

        auto future = std::async(fold < decltype(range), T, Op > , range, T(), op);

        T right_part = fold(std::ranges::subrange(std::end(range), end), T(), op);

        return op(init, op(future.get(), right_part));
    }
    else
    {
        auto res = std::ranges::fold_left_first(view, op);

        return res ? op(init, *res) : init;
    }
}
///////////////////////////////////////////////////////////////////////////

// fold returns T, the computed result
// std::for_each returns _Function, the function object you pass in

TEST(FoldTests, Sum)
{
    std::vector<int> vector(1 << 10);

    std::ranges::iota(vector, 1);

    int result = fold(std::views::all(vector), 0, std::plus<int>());

    EXPECT_EQ(result, 524800);
}

TEST(FoldTests, Max) {
    std::vector<int> vector(1 << 10);

    std::ranges::iota(vector, 1);

    int init = std::numeric_limits<int>::min();

    int result = fold(std::views::all(vector), init, [](int a, int b) { return std::max(a, b); });

    EXPECT_EQ(result, 1024);
}

TEST(FoldTests, Product) {
    std::vector<int> vector(5);

    std::ranges::iota(vector, 1);

    int result = fold(std::views::all(vector), 1, std::multiplies<int>());

    EXPECT_EQ(result, 120);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
