#include <gtest/gtest.h>
#include <variant>
#include <utility>
#include <cmath>

using Roots = std::variant<std::monostate, bool, double, std::pair<double, double>>;

bool equals(double x, double y, double epsilon = 1e-7)
{
    return std::abs(x - y) < epsilon;
}

Roots solve(double a, double b, double c)
{
    if (equals(a, 0.0))
    {
        if (equals(b, 0.0))
        {
            if (equals(c, 0.0))
            {
                return std::monostate{};
            }
            return false;
        }
        return -c / b;
    }

    double D = b * b - 4 * a * c;
    if (equals(D, 0.0))
    {
        return -b / (2 * a);
    }

    if (D > 1e-7)
    {
        double sqrtD = std::sqrt(D);
        return std::pair{(-b + sqrtD) / (2 * a), (-b - sqrtD) / (2 * a)};
    }
    return false;
}

struct Visitor
{
    enum class State { UNKNOWN, INF, NONE, ONE, TWO };

    State current_state = State::UNKNOWN;
    double x1 = 0.0;
    double x2 = 0.0;

    void operator()(std::monostate)
    {
        current_state = State::INF;
    }

    void operator()(bool)
    {
        current_state = State::NONE;
    }

    void operator()(double r)
    {
        current_state = State::ONE;
        x1 = x2 = r;
    }

    void operator()(const std::pair<double, double>& p)
    {
        current_state = State::TWO;
        x1 = std::max(p.first, p.second);
        x2 = std::min(p.first, p.second);
    }
};

class QuadraticSolverTest : public ::testing::Test
{
protected:
    Visitor v;

    void check(double a, double b, double c)
    {
        std::visit(v, solve(a, b, c));
    }
};

TEST_F(QuadraticSolverTest, InfiniteRoots)
{
    // 0*x^2 + 0*x + 0 = 0
    check(0.0, 0.0, 0.0);
    EXPECT_EQ(v.current_state, Visitor::State::INF);
}

TEST_F(QuadraticSolverTest, NoRoots)
{
    // 1*x^2 + 0*x + 1 = 0
    check(1.0, 0.0, 1.0);
    EXPECT_EQ(v.current_state, Visitor::State::NONE);
}

TEST_F(QuadraticSolverTest, OneRootLinear)
{
    // 0*x^2 + 2*x - 4 = 0
    check(0.0, 2.0, -4.0);
    EXPECT_EQ(v.current_state, Visitor::State::ONE);
    EXPECT_NEAR(v.x1, 2.0, 1e-7);
    EXPECT_NEAR(v.x2, 2.0, 1e-7);
}

TEST_F(QuadraticSolverTest, OneRootQuadratic)
{
    // 1*x^2 - 2*x + 1 = 0
    check(1.0, -2.0, 1.0);
    EXPECT_EQ(v.current_state, Visitor::State::ONE);
    EXPECT_NEAR(v.x1, 1.0, 1e-7);
    EXPECT_NEAR(v.x2, 1.0, 1e-7);
}

TEST_F(QuadraticSolverTest, TwoRoots)
{
    // 1*x^2 - 5*x + 6 = 0
    check(1.0, -5.0, 6.0);
    EXPECT_EQ(v.current_state, Visitor::State::TWO);
    EXPECT_NEAR(v.x1, 3.0, 1e-7);
    EXPECT_NEAR(v.x2, 2.0, 1e-7);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}