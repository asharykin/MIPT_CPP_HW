#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cmath>
#include <random>
#include <limits>

// 1) Fibonacci View implemented via view_interface
class Fibonacci : public std::ranges::view_interface<Fibonacci> {
private:
    class Iterator {
    public:
        using iterator_concept = std::forward_iterator_tag;
        using iterator_category = std::forward_iterator_tag;
        using value_type = size_t;
        using difference_type = std::ptrdiff_t;

        Iterator() = default;
        explicit Iterator(size_t limit) : max_index(limit) {}

        value_type operator*() const { return current; }

        Iterator& operator++() {
            size_t next_val = current + next;
            current = next;
            next = next_val;
            index++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return index == other.index;
        }

        // Enable comparison with sentinel
        bool operator==(std::default_sentinel_t) const {
            return index >= max_index;
        }

    private:
        size_t current = 0;
        size_t next = 1;
        size_t index = 0;
        size_t max_index = std::numeric_limits<size_t>::max();
    };

    size_t count;

public:
    Fibonacci() : count(std::numeric_limits<size_t>::max()) {}
    explicit Fibonacci(size_t n) : count(n) {}

    Iterator begin() const { return Iterator(count); }
    std::default_sentinel_t end() const { return std::default_sentinel; }
};

// 2) MAE and MSE without relying on zip (C++23)

template <std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mae(R1&& r1, R2&& r2) {
    auto it1 = std::begin(r1);
    auto end1 = std::end(r1);
    auto it2 = std::begin(r2);
    auto end2 = std::end(r2);

    double sum = 0.0;
    size_t count = 0;
    while (it1 != end1 && it2 != end2) {
        sum += std::abs(static_cast<double>(*it1) - static_cast<double>(*it2));
        ++it1; ++it2; ++count;
    }
    return count ? sum / static_cast<double>(count) : 0.0;
}

template <std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mse(R1&& r1, R2&& r2) {
    auto it1 = std::begin(r1);
    auto end1 = std::end(r1);
    auto it2 = std::begin(r2);
    auto end2 = std::end(r2);

    double sum = 0.0;
    size_t count = 0;
    while (it1 != end1 && it2 != end2) {
        double diff = static_cast<double>(*it1) - static_cast<double>(*it2);
        sum += diff * diff;
        ++it1; ++it2; ++count;
    }
    return count ? sum / static_cast<double>(count) : 0.0;
}

// 3) Tests

TEST(RangesAlgoTest, StandardAlgorithms) {
    // ranges::replace
    std::vector<int> v = {1, 2, 2, 3};
    std::ranges::replace(v, 2, 5);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 5);

    // ranges::fill
    std::ranges::fill(v, 10);
    for (int n : v) EXPECT_EQ(n, 10);

    // ranges::unique
    std::vector<int> v2 = {1, 1, 2, 2, 3};
    auto [last, end] = std::ranges::unique(v2);
    v2.erase(last, end);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[1], 2);

    // ranges::rotate
    std::vector<int> v3 = {1, 2, 3, 4};
    std::ranges::rotate(v3, v3.begin() + 1);
    EXPECT_EQ(v3[0], 2);
    EXPECT_EQ(v3[3], 1);

    // ranges::sample
    std::vector<int> v4 = {1,2,3,4,5};
    std::vector<int> out;
    std::mt19937 gen(42);
    std::ranges::sample(v4, std::back_inserter(out), 3, gen);
    EXPECT_EQ(out.size(), 3);
}

TEST(RangesViewsTest, PipeCompositions) {
    std::vector<int> data = {1,2,3,4,5,6,7,8,9,10};
    // filter, drop, stride
    auto res = data
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::drop(1)
        | std::views::stride(2)
        | std::views::common;

    std::vector<int> vec(res.begin(), res.end());

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 4);
    EXPECT_EQ(vec[1], 8);
}

TEST(FibonacciTest, ViewInterfaceImplementation) {
    Fibonacci fib(7); // 0, 1, 1, 2, 3, 5, 8

    std::vector<size_t> expected = {0, 1, 1, 2, 3, 5, 8};
    std::vector<size_t> actual;
    for (auto n : fib) actual.push_back(n);
    EXPECT_EQ(actual, expected);

    auto even_fib = fib | std::views::filter([](size_t n){ return n % 2 == 0; }) | std::views::common;
    std::vector<size_t> even_actual(even_fib.begin(), even_fib.end());
    std::vector<size_t> even_expected = {0, 2, 8};
    EXPECT_EQ(even_actual, even_expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}