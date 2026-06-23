#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <gtest/gtest.h>

std::string bytes_to_hexstr(const std::vector<unsigned char>& bytes)
{
    std::ostringstream oss;
    for (const auto byte : bytes)
    {
        oss << std::right << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

unsigned char hexchar_to_int(char const ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }

    if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }

    throw std::invalid_argument("Invalid hexadecimal character");
}

std::vector<unsigned char> hexstr_to_bytes(std::string_view str)
{
    std::vector<unsigned char> result;
    size_t i = 0;
    if (str.size() & 1)
    {
        result.push_back(hexchar_to_int(str[i]));
        i = 1;
    }
    for (; i < str.size(); i += 2)
    {
        result.push_back((hexchar_to_int(str[i]) << 4) | hexchar_to_int(str[i + 1]));
    }
    return result;
}

TEST(HexConverterTest, BytesToHexStr)
{
    std::vector<unsigned char> v_hex { 0xba, 0xad, 0xf0, 0x0d, 0x42 };
    EXPECT_EQ(bytes_to_hexstr(v_hex), "baadf00d42");

    std::vector<unsigned char> v_dec { 186, 173, 240, 13, 65 };
    EXPECT_EQ(bytes_to_hexstr(v_dec), "baadf00d41");
}

TEST(HexConverterTest, HexStrToBytes)
{
    std::vector<unsigned char> expected_hex { 0xba, 0xad, 0xf0, 0x0d, 0x42 };
    EXPECT_EQ(hexstr_to_bytes("baadf00d42"), expected_hex);

    std::vector<unsigned char> expected_dec { 186, 173, 240, 13, 65 };
    EXPECT_EQ(hexstr_to_bytes("baadf00d41"), expected_dec);
}

TEST(HexConverterTest, InvalidInput)
{
    EXPECT_THROW(hexstr_to_bytes("????"), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
