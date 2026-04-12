////////////////////////////////////////////////////////////////////////////////////

// chapter : Characters

////////////////////////////////////////////////////////////////////////////////////

// section : Regular Expressions

////////////////////////////////////////////////////////////////////////////////////

// content : Iterator std::sregex_token_iterator

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iterator>
#include <regex>
#include <string>
#include <vector>
#include <gtest/gtest.h>

////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> extract_emails(const std::string& string, const std::vector<int>& tokens = {0})
{
    std::regex pattern(R"(([a-z0-9._-]+)@([a-z]+)\.([a-z]+))", std::regex_constants::icase);

    //  --------------------------------------------------------------------------------

    std::vector<std::string> strings;

    //  --------------------------------------------------------------------------------

    auto lambda = [&strings](auto const & match){ strings.push_back(match); };

    //  --------------------------------------------------------------------------------

    std::ranges::for_each
    (
        std::sregex_token_iterator(std::cbegin(string), std::cend(string), pattern, tokens),

        std::sregex_token_iterator(),

        lambda
    );

    //  --------------------------------------------------------------------------------

    return strings;
}

////////////////////////////////////////////////////////////////////////////////////

TEST(EmailExtractorTest, HandlesValidEmails)
{
    auto string = R"(
                      First e-mail: aleksandr-sharykin@mail.ru;
                      Second e-mail: avsharykin@yandex.ru;
                    )"s;
    std::vector<std::string> expected = {"aleksandr-sharykin@mail.ru", "avsharykin@yandex.ru"};

    EXPECT_EQ(extract_emails(string), expected);
}

////////////////////////////////////////////////////////////////////////////////////

TEST(EmailExtractorTest, ExtractsOnlyDomains)
{
    auto string = R"(
                      First e-mail: avsharykin@yandex.ru;
                      Second e-mail: aleksandrsharykin@gmail.com;
                    )"s;
    std::vector<std::string> expected = {"ru", "com"};

    EXPECT_EQ(extract_emails(string, {3}), expected);
}

////////////////////////////////////////////////////////////////////////////////////

TEST(EmailExtractorTest, HandlesMixedCase)
{
    auto string = R"(
                      First e-mail: AVSharykin@yandex.ru
                    )"s;
    std::vector<std::string> expected = {"AVSharykin@yandex.ru"};

    EXPECT_EQ(extract_emails(string), expected);
}

////////////////////////////////////////////////////////////////////////////////////

TEST(EmailExtractorTest, ReturnsEmptyForInvalidInput)
{
    auto string = R"(
                      First non e-mail: not-an-email;
                      Second non e-mail: @yandex.ru;
                      Third non e-mail: avsharykin@yandex;
                    )"s;

    EXPECT_TRUE(extract_emails(string).empty());
}

////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}