#include <iostream>
#include <cmath>

double calculate_max(double arr[], int n)
{
    double max = arr[0];
    for (int i = 1; i < n; ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

double calculate_min(double arr[], int n)
{
    double min = arr[0];
    for (int i = 1; i < n; ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

void bubble_sort(double arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

double calculate_median(double arr[], int n)
{
    bubble_sort(arr, n);
    if (n % 2 == 0)
    {
        return (arr[n / 2 - 1] + arr[n / 2]) / 2;
    }
    return arr[n / 2];
}

double calculate_mean(double arr[], int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }
    return sum / n;
}

double calculate_standard_deviation(double arr[], int n)
{
    double mean = calculate_mean(arr, n);
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sum += pow(arr[i] - mean, 2);
    }
    return sqrt(sum / (n - 1));
}

int main()
{
    std::cout << "Enter collection size: " << std::endl;
    int n;
    std::cin >> n;

    if (n <= 0)
    {
        std::cerr << "Incorrect collection size" << std::endl;
        return 1;
    }

    double numbers[n];
    std::cout << "Enter " << n << " numbers (space-separated): " << std::endl;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> numbers[i];
    }

    double max = calculate_max(numbers, n);
    double min = calculate_min(numbers, n);
    double mean = calculate_mean(numbers, n);
    double median = calculate_median(numbers, n);
    double std_dev = calculate_standard_deviation(numbers, n);

    std::cout << "max: " << max << std::endl;
    std::cout << "min: " << min << std::endl;
    std::cout << "median: " << median << std::endl;
    std::cout << "mean: " << mean << std::endl;
    std::cout << "standard deviation: " << std_dev << std::endl;
}
