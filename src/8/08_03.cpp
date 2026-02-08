#include <cassert>
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
        if (res < 0 && mantissa != 0) {
            res += 1;
        }
    } else {
        // Denormalized numbers (very-very-very small)
        unsigned int temp = mantissa;
        int msb_position = -1;

        while (temp > 0) {
            temp = temp >> 1;
            msb_position++;
        }

        res = -126 + (msb_position - 23);

        // x is not power of 2
        if (mantissa > (1u << msb_position)) {
            res += 1;
        }
    }

    return res;
}

int main()
{
    try {
        log2_custom(-2);
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Input must be a positive integer.");
    }

    assert(log2_custom(1) == 0);
    assert(log2_custom(2) == 1);
    assert(log2_custom(10) == 3);
    assert(log2_custom(20) == 4);

    float inf = 1.0f / 0.0f;
    try {
        log2_custom(inf);
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Input cannot be Infinity or NaN.");
    }

    float nan = 0.0f / 0.0f;
    try {
        log2_custom(nan);
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Input cannot be Infinity or NaN.");
    }

    assert(log2_custom(10.75f) == 3);
    assert(log2_custom(2.5f) == 1);
    assert(log2_custom(0.9f) == 0);
    assert(log2_custom(0.25f) == -2);
    assert(log2_custom(0.01f) == -6);
    assert(log2_custom(0.00000000000000000000000000000000000001f) == -126);
    assert(log2_custom(static_cast<float>(std::pow(2, -130))) == -130);
}