#include <iostream>
#include <vector>

std::size_t compute_collatz_length_for_i(unsigned long long int i, std::vector<std::size_t>& cache)
{
    std::size_t length = 0;
    unsigned long long n = i;

    while (n != 1 && n >= i)
    {
        if (n % 2 == 0)
        {
            n = n / 2;
        }
        else
        {
            n = n * 3 + 1;
        }
        length++;
    }

    length += cache[n - 1];
    cache[i - 1] = length;
    return length;
}

int main()
{
    std::size_t max_length = 0;
    unsigned long long max_length_n = 0;

    std::vector<std::size_t> cache(100, 1);
    for (unsigned long long i = 1; i <= 100; i++)
    {
        compute_collatz_length_for_i(i, cache);
        if (cache[i - 1] > max_length)
        {
            max_length = cache[i - 1];
            max_length_n = i;
        }
    }
    std::cout << "Length of the longest Collatz conjecture: "<< max_length << std::endl;
    std::cout << "Starting number of the longest Collatz conjecture: " << max_length_n << std::endl;
}