#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>
#include <ranges>

template <typename T> void order(T& vector, std::size_t left, std::size_t right)
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

template <typename T> std::size_t partition(T& vector, std::size_t left, std::size_t right)
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

template <typename T> void quick_split(T& vector, std::size_t left, std::size_t right)
{
    if (right - left <= 64)
    {
        order(vector, left, right);
        return;
    }

    std::size_t pivot_index = partition(vector, left, right);

    if (pivot_index > left) quick_split(vector, left, pivot_index);
    if (pivot_index + 1 < right) quick_split(vector, pivot_index + 1, right);
}

template <typename T> void sort(T& vector)
{
    quick_split(vector, 0, vector.size());
}

int main()
{
    int size = 1000;
    std::vector<int> vector(size);
    for (auto i = 0; i < size; i++)
    {
        vector[i] = size - i;
    }
    sort(vector);
    assert(std::ranges::is_sorted(vector));
}
