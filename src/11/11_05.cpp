#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <random>
#include <gtest/gtest.h>


TEST(RangesAlgorithmsTest, ReplaceTest)
{
    std::vector<int> v = {1, 2, 3, 2, 4};
    std::ranges::replace(v, 2, 99);

    std::vector<int> expected = {1, 99, 3, 99, 4};
    EXPECT_EQ(v, expected);
}

TEST(RangesAlgorithmsTest, FillTest)
{
    std::vector<int> v(5);
    std::ranges::fill(v, 10);

    for (int n : v)
    {
        EXPECT_EQ(n, 10);
    }
}

TEST(RangesAlgorithmsTest, UniqueTest)
{
    std::vector<int> v = {1, 1, 2, 2, 3, 1, 1};
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);

    std::vector<int> expected = {1, 2, 3, 1};
    EXPECT_EQ(v, expected);
}

TEST(RangesAlgorithmsTest, RotateTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = std::ranges::find(v, 3);
    std::ranges::rotate(v, it);

    std::vector<int> expected = {3, 4, 5, 1, 2};
    EXPECT_EQ(v, expected);
}

TEST(RangesAlgorithmsTest, SampleTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> out;
    std::mt19937 gen{std::random_device{}()};

    std::ranges::sample(v, std::back_inserter(out), 3, gen);

    EXPECT_EQ(out.size(), 3);
    for (int n : out)
    {
        EXPECT_TRUE(std::ranges::contains(v, n));
    }
}

auto transform_if(const std::vector<int>& r, std::back_insert_iterator<std::vector<int>> result,
                  bool (*pred)(int), int (*op)(int)) {
    std::vector<int> temp;
    std::ranges::copy_if(r, std::back_inserter(temp), pred);
    return std::ranges::transform(temp, result, op).out;
}

TEST(CustomAlgorithmsTest, TransformIfCombinedTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::vector<int> result;

    transform_if(v, std::back_inserter(result),
                 [](int n) { return n % 2 == 0; },
                 [](int n) { return n * n; }
    );

    std::vector<int> expected = {4, 16, 36};
    EXPECT_EQ(result, expected);
}

double calculate_mae(const std::vector<double>& actual, const std::vector<double>& predicted)
{
    if (actual.empty() || actual.size() != predicted.size()) return 0.0;
    std::vector<double> diffs;
    diffs.reserve(actual.size());
    std::transform(actual.begin(), actual.end(), predicted.begin(), std::back_inserter(diffs),
                   [](double a, double p) { return std::abs(a - p); });
    double sum = std::accumulate(diffs.begin(), diffs.end(), 0.0);
    return sum / actual.size();
}

double calculate_mse(const std::vector<double>& actual, const std::vector<double>& predicted) {
    if (actual.empty() || actual.size() != predicted.size()) return 0.0;
    std::vector<double> sq_diffs;
    sq_diffs.reserve(actual.size());
    std::transform(actual.begin(), actual.end(), predicted.begin(), std::back_inserter(sq_diffs),
                   [](double a, double p) {
                       double d = a - p;
                       return d * d;
                   });
    double sum = std::accumulate(sq_diffs.begin(), sq_diffs.end(), 0.0);
    return sum / actual.size();
}

TEST(ErrorMetricsTest, MAETest) {
    std::vector<double> actual = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> predicted = {1.5, 2.5, 2.5, 4.5};

    double mae = calculate_mae(actual, predicted);
    EXPECT_NEAR(mae, 0.5, 1e-9);
}

TEST(ErrorMetricsTest, MSETest) {
    std::vector<double> actual = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> predicted = {1.5, 2.5, 2.5, 4.5};

    double mse = calculate_mse(actual, predicted);
    EXPECT_NEAR(mse, 0.25, 1e-9);
}

TEST(RangesViewsTest, FilterTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto view = v | std::views::filter([](int n) { return n % 2 == 0; });

    std::vector<int> result(view.begin(), view.end());
    std::vector<int> expected = {2, 4, 6};
    EXPECT_EQ(result, expected);
}

TEST(RangesViewsTest, DropTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto view = v | std::views::drop(2);

    std::vector<int> result(view.begin(), view.end());
    std::vector<int> expected = {3, 4, 5};
    EXPECT_EQ(result, expected);
}

TEST(RangesViewsTest, JoinTest)
{
    std::vector<std::string> words = {"Hello", " ", "World"};
    auto view = words | std::views::join;

    std::string result(view.begin(), view.end());
    EXPECT_EQ(result, "Hello World");
}

TEST(RangesViewsTest, ZipTest)
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<std::string> v2 = {"one", "two", "three"};

    auto view = std::views::zip(v1, v2);

    auto it = view.begin();
    auto [val_int, val_str] = *it;

    EXPECT_EQ(val_int, 1);
    EXPECT_EQ(val_str, "one");

    ++it;
    auto [val_int2, val_str2] = *it;
    EXPECT_EQ(val_int2, 2);
    EXPECT_EQ(val_str2, "two");
}

TEST(RangesViewsTest, StrideTest)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
    auto view = v | std::views::stride(3);

    std::vector<int> result(view.begin(), view.end());
    std::vector<int> expected = {1, 4, 7};
    EXPECT_EQ(result, expected);
}

class Fibonacci : public std::ranges::view_interface<Fibonacci>
{
private:

    class Iterator
    {
    public:

        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        Iterator() : m_previous(0), m_current(1), m_count(0) { }

        explicit Iterator(int limit) : m_previous(0), m_current(1), m_count(limit) { }

        int operator*() const
        {
            return m_current;
        }

        Iterator& operator++()
        {
            int temp = m_previous + m_current;
            m_previous = m_current;
            m_current = temp;
            m_count--;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const
        {
            return m_count == other.m_count;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

    private:

        int m_previous;
        int m_current;
        int m_count;
    };

public:

    Fibonacci() : m_limit(-1) { }

    explicit Fibonacci(int limit) : m_limit(limit) { }

    Iterator begin() const
    {
        return Iterator(m_limit);
    }

    Iterator end() const
    {
        return Iterator(0);
    }

private:

    int m_limit;
};

TEST(FibonacciViewTest, SequenceTest)
{
    Fibonacci fib(7);
    std::vector<int> result;
    for (int val : fib)
    {
        result.push_back(val);
    }
    std::vector<int> expected = {1, 1, 2, 3, 5, 8, 13};
    EXPECT_EQ(result, expected);
}

TEST(FibonacciViewTest, ViewInterfaceTest)
{
    Fibonacci fib(5);
    EXPECT_FALSE(fib.empty());
    EXPECT_EQ(fib.front(), 1);
}

TEST(FibonacciViewTest, RangeAdaptorTest)
{
    Fibonacci fib(10);
    auto even_fibs = fib | std::views::filter([](int n) { return n % 2 == 0; });

    std::vector<int> result;
    for (int val : even_fibs)
    {
        result.push_back(val);
    }
    std::vector<int> expected = {2, 8, 34};
    EXPECT_EQ(result, expected);
}

TEST(FibonacciViewTest, TakeTest)
{
    Fibonacci fib(-1);
    auto first_five = fib | std::views::take(5);

    std::vector<int> result;
    for (int val : first_five)
    {
        result.push_back(val);
    }
    std::vector<int> expected = {1, 1, 2, 3, 5};
    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}