#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <print>
#include <string>
#include <vector>

template <typename D = std::chrono::duration<double>>
class Timer
{
public:

    Timer(std::string scope) : m_scope(std::move(scope)), m_running(false) { }

    ~Timer()
    {
        std::cout << std::format("{} (average) : {:.6f}s\n", m_scope, average());
    }

    void start()
    {
        if (!m_running)
        {
            m_begin = clock_t::now();
            m_running = true;
        }
    }

    void stop()
    {
        if (m_running)
        {
            m_measurements.push_back(std::chrono::duration_cast<D>(clock_t::now() - m_begin));
            m_running = false;
        }
    }

    double average() const
    {
        if (m_measurements.empty()) return 0.0;

        D total_duration{0};
        for (const auto& duration : m_measurements)
        {
            total_duration += duration;
        }

        return total_duration.count() / count();
    }

    std::size_t count() const
    {
        return m_measurements.size();
    }

private:
    using clock_t = std::chrono::steady_clock;

    std::string m_scope;
    clock_t::time_point m_begin;
    bool m_running;
    std::vector<D> m_measurements;
};

////////////////////////////////////////////////////////////////////////////////////

auto calculate(std::size_t size)
{
    auto x = 0.0;
    for (auto i = 0uz; i < size; ++i)
    {
        x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
    }
    return x;
}

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    Timer timer("Calculations Series");

    for (int i = 0; i < 5; ++i)
    {
        timer.start();
        calculate(1'000'000);
        timer.stop();
    }
}