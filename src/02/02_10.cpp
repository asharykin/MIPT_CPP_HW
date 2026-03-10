#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

void order(std::vector<int>& vector, std::size_t left, std::size_t right) // insertion sort for small arrays
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

std::size_t partition(std::vector<int>& vector, std::size_t left, std::size_t right)
{
	// take the last element as pivot
	int pivot = vector[right - 1];
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
	return i; // pivot index after partitioning
}

void quick_split(std::vector<int>& vector, std::size_t left, std::size_t right) // recursive quick sort function
{
	if (right - left <= 64)
	{
		order(vector, left, right); // insertion sort for small sections
		return;
	}
	std::size_t pivot_index = partition(vector, left, right);
	if (pivot_index > left) quick_split(vector, left, pivot_index);
	if (pivot_index + 1 < right) quick_split(vector, pivot_index + 1, right);
}

void sort(std::vector<int>& vector)
{
	quick_split(vector, 0, vector.size());
}

int main()
{
	auto size = 1'000uz;
	std::vector<int> vector(size, 0);
	for (auto i = 0uz; i < size; ++i)
	{
		vector[i] = size - i;
	}
	sort(vector);
	assert(std::ranges::is_sorted(vector));
}

// The complexity of quick sort is O(n*logn), of insertion sort - O(k*n)
// But when k is small enough (<=64), the complexity of insertion sort is O(64*n) = O(n)
// Thus, the complexity of the whole algorithm is O(n*logn) + O(n) = O(n*logn)