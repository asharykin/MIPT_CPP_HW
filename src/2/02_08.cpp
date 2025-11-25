#include <iostream>
#include <vector>

std::size_t collatz_length(unsigned long long int i, std::vector<std::size_t>& cache)
{
    unsigned long long n = i;
    std::size_t length = 0;
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
    length += cache[n];
    cache[i] = length;
    return length;
}

int main() {
    unsigned long long limit = 100;
    std::size_t max_length = 0;
    unsigned long long max_length_n = 0;
    std::vector<std::size_t> cache(limit + 1, 0);
    for (unsigned long long i = 2; i <= limit; i++)
    {
        collatz_length(i, cache);
        if (cache[i] > max_length)
        {
            max_length = cache[i];
            max_length_n = i;
        }
    }
    std::cout << "Length of the longest Collatz conjecture: "<< max_length << std::endl;
    std::cout << "Starting number of the longest Collatz conjecture: " << max_length_n << std::endl;
}