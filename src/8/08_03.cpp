#include <gtest/gtest.h>
#include <complex>
#include <stdexcept>

int log2_custom(int x)
{
    if (x <= 0)
    {
        throw std::invalid_argument("Input must be a positive integer.");
    }

    unsigned int val = static_cast<unsigned int>(x);
    int msb_position = -1;

    while (val > 0)
    {
        val = val >> 1;
        msb_position++;
    }
    return msb_position;
}

int log2_custom(float x)
{
    union
    {
        float f_val;
        unsigned int u_val;
    } converter;

    converter.f_val = x;
    unsigned int u = converter.u_val;

    // Extract sign (1 bit), exponent (8 bits) and mantissa (23 bits) using bitwise operations
    unsigned int sign = (u >> 31) & 0x1;
    unsigned int exponent = (u >> 23) & 0xFF;
    unsigned int mantissa = u & 0x7FFFFF;

    // Infinity or NaN
    if (exponent == 255)
    {
        throw std::invalid_argument("Input cannot be Infinity or NaN.");
    }

    // Negative numbers or zero
    if (sign == 1 || (exponent == 0 && mantissa == 0))
    {
        throw std::invalid_argument("Input must be a positive number.");
    }

    int res;

    if (exponent > 0) {
        // Normalized numbers
        res = static_cast<int>(exponent) - 127;

        // For 0 < x < 1, x not power of 2
        if (res < 0 && mantissa != 0)
        {
            res += 1;
        }
    } else {
        // Denormalized numbers (very-very-very small)
        unsigned int temp = mantissa;
        int msb_position = -1;

        while (temp > 0)
        {
            temp = temp >> 1;
            msb_position++;
        }

        res = -126 + (msb_position - 23);

        // x is not power of 2
        if (mantissa > (1u << msb_position))
        {
            res += 1;
        }
    }

    return res;
}

TEST(Log2CustomTest, ThrowsOnInvalidArguments)
{
    EXPECT_THROW(log2_custom(-2), std::invalid_argument);

    float inf = 1.0f / 0.0f;
    EXPECT_THROW(log2_custom(inf), std::invalid_argument);

    float nan = 0.0f / 0.0f;
    EXPECT_THROW(log2_custom(nan), std::invalid_argument);
}

TEST(Log2CustomTest, PositiveIntegers)
{
    EXPECT_EQ(log2_custom(1), 0);
    EXPECT_EQ(log2_custom(2), 1);
    EXPECT_EQ(log2_custom(10), 3);
    EXPECT_EQ(log2_custom(20), 4);
}

TEST(Log2CustomTest, FloatingPointValues)
{
    EXPECT_EQ(log2_custom(10.75f), 3);
    EXPECT_EQ(log2_custom(2.5f), 1);
    EXPECT_EQ(log2_custom(0.9f), 0);
    EXPECT_EQ(log2_custom(0.25f), -2);
    EXPECT_EQ(log2_custom(0.01f), -6);
}

TEST(Log2CustomTest, ExtremeValues)
{
    EXPECT_EQ(log2_custom(0.00000000000000000000000000000000000001f), -126);
    EXPECT_EQ(log2_custom(static_cast<float>(std::pow(2, -130))), -130);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}