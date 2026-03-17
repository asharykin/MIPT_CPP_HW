////////////////////////////////////////////////////////////////////////////////////////////

// chapter : Number Processing

////////////////////////////////////////////////////////////////////////////////////////////

// section : Long Arithmetic

////////////////////////////////////////////////////////////////////////////////////////////

// content : Function main

////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <sstream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

////////////////////////////////////////////////////////////////////////////////////////////

#include "integer.hpp"

class IntegerTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
        x = std::string(32, '1');
        y = std::string(32, '2');
        zero = "0"s;
    }

    Integer x;
    Integer y;
    Integer zero;
};

TEST_F(IntegerTest, CompoundAssignmentOperators)
{
    EXPECT_EQ((x += y), "+33333333333333333333333333333333"s);
    EXPECT_EQ((x -= y), "+11111111111111111111111111111111"s);
    EXPECT_EQ((x *= y), "+246913580246913580246913580246908641975308641975308641975308642"s);
    EXPECT_EQ((x /= y), "+11111111111111111111111111111111"s);
}

TEST_F(IntegerTest, IncrementDecrementOperators)
{
    EXPECT_EQ((x++), "+11111111111111111111111111111111"s);
    EXPECT_EQ((x--), "+11111111111111111111111111111112"s);
    EXPECT_EQ((++y), "+22222222222222222222222222222223"s);
    EXPECT_EQ((--y), "+22222222222222222222222222222222"s);
}

TEST_F(IntegerTest, ArithmeticOperators)
{
    EXPECT_EQ((x + y), "+33333333333333333333333333333333"s);
    EXPECT_EQ((x - y), "-11111111111111111111111111111111"s);
    EXPECT_EQ((x * y), "+246913580246913580246913580246908641975308641975308641975308642"s);
    EXPECT_EQ((x / y), zero);
}

TEST_F(IntegerTest, ComparisonOperators)
{
    EXPECT_TRUE(x < y);
    EXPECT_FALSE(x > y);
    EXPECT_TRUE(x <= y);
    EXPECT_FALSE(x >= y);
    EXPECT_FALSE(x == y);
    EXPECT_TRUE(x != y);
}

TEST_F(IntegerTest, StreamOperators)
{
    std::stringstream stream_1( std::string(32, '1'));
    std::stringstream stream_2;

    stream_1 >> x;
    stream_2 << x;

    EXPECT_EQ(stream_2.str(), stream_1.str());
}

TEST_F(IntegerTest, MathematicalFunctions)
{
    EXPECT_EQ(sqrt(multiply(x, x)), x);

    EXPECT_EQ((x - y).abs(), x);
    EXPECT_EQ(x.abs(), x);
    EXPECT_EQ(zero.abs(), zero);

    EXPECT_EQ((x - y).sign(), -1);
    EXPECT_EQ(x.sign(), 1);
    EXPECT_EQ(zero.sign(), 0);

    EXPECT_EQ((y % x), zero);
    EXPECT_EQ((y %= "+1000"s), "+222"s);
    EXPECT_EQ((y % "+100"s), "+22"s);

    Integer base_2 = "+2"s;
    EXPECT_EQ(base_2.pow(10), "+1024"s);
    EXPECT_EQ(base_2.pow(0), "+1"s);

    Integer base_10 = "+10"s;
    EXPECT_EQ(base_10.pow(20), "+100000000000000000000"s);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}