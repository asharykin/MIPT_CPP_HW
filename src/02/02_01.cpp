#include <iostream>
#include <cmath>

int main() {
    const double phi = (1 + std::sqrt(5)) / 2;
    const double sqrt5 = std::sqrt(5);

    int N;
    std::cout << "Enter N: " << std::endl;
    std::cin >> N;

    double fib = (std::pow(phi, N) - std::pow(1 - phi, N)) / sqrt5;
    int result = static_cast<int>(std::round(fib));
    std::cout << "F(" << N << ") = " << result << std::endl;
}
