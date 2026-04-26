#include <gtest/gtest.h>
#include <random>
#include <thread>
#include <vector>
#include <atomic>

double monte_carlo_pi_estimate(long long total_iterations, unsigned int num_threads = std::thread::hardware_concurrency())
{
    std::atomic<long long> total_inside_points(0);

    long long base = total_iterations / num_threads;

    long long remainder = total_iterations % num_threads;

    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < num_threads; ++i)
    {
        long long local_iterations = base + (i < remainder ? 1 : 0);

        threads.emplace_back([&total_inside_points, local_iterations]()
        {
            std::random_device rd;

            std::mt19937 gen(rd());

            std::uniform_real_distribution<double> dist(0.0, 1.0);

            long long local_inside_points = 0;
            for (long long k = 0; k < local_iterations; ++k)
            {
                double x = dist(gen);

                double y = dist(gen);

                if (x * x + y * y <= 1.0)
                {
                    ++local_inside_points;
                }
            }
            total_inside_points += local_inside_points;
        });
    }

    for (auto& t : threads)
    {
        t.join();
    }

    return 4.0 * static_cast<double>(total_inside_points) / total_iterations;
}

class MonteCarloPiTestFixture : public ::testing::Test
{
protected:

    static constexpr double PI = 3.14159265358979323846;
};

TEST_F(MonteCarloPiTestFixture, OneMillionPoints)
{
    double pi = monte_carlo_pi_estimate(1'000'000);

    EXPECT_NEAR(pi, PI, 0.005);
}

TEST_F(MonteCarloPiTestFixture, TenMillionPoints)
{
    double pi = monte_carlo_pi_estimate(10'000'000);

    EXPECT_NEAR(pi, PI, 0.001);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}