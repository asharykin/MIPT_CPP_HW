#include <iostream>
#include <vector>
#include <cstddef>

#include <benchmark/benchmark.h>

template <typename T> void order(std::vector<T>& vector, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
    {
        for (auto j = i; j > left; --j)
        {
            if (vector[j - 1] > vector[j])
            {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template <typename T> std::size_t partition(std::vector<T>& vector, std::size_t left, std::size_t right)
{
    auto pivot = vector[right - 1];
    std::size_t i = left;

    for (std::size_t j = left; j < right - 1; ++j)
    {
        if (vector[j] <= pivot)
        {
            std::swap(vector[i], vector[j]);
            ++i;
        }
    }
    std::swap(vector[i], vector[right - 1]);
    return i;
}

template <typename T>void quick_split(std::vector<T>& vector, std::size_t left, std::size_t right, std::size_t threshold)
{
    if (right - left <= threshold)
    {
        order(vector, left, right);
        return;
    }

    std::size_t pivot_index = partition(vector, left, right);

    if (pivot_index > left) quick_split(vector, left, pivot_index, threshold);
    if (pivot_index + 1 < right) quick_split(vector, pivot_index + 1, right, threshold);
}

template <typename T> void sort(std::vector<T>& vector, std::size_t threshold)
{
    quick_split(vector, 0, vector.size(), threshold);
}

void benchmark_sort(benchmark::State& state)
{
    std::size_t threshold = state.range(0);
    std::vector<double> vec(100000);

    for (std::size_t i = 0; i < vec.size(); i++)
    {
        vec[i] = 100000 - i;
    }

    for (auto element : state)
    {
        sort(vec, threshold);
        // benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_sort)->RangeMultiplier(2)->Range(16, 256);

// It might be a good idea to increase the stack size in your OS before launching the app
// For me, 10 MiB was enough
int main()
{
    benchmark::RunSpecifiedBenchmarks();
}