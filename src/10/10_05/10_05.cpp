#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <unordered_set>
#include <functional>
#include <fstream>

using HashFunc = std::function<unsigned int(const std::string&)>;

unsigned int RSHash(const std::string& str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    for (char c : str)
    {
        hash = hash * a + static_cast<unsigned int>(c);
        a *= b;
    }
    return hash;
}

unsigned int JSHash(const std::string& str)
{
    unsigned int hash = 1315423911;
    for (char c : str)
    {
        hash ^= ((hash << 5) + c + (hash >> 2));
    }
    return hash;
}

unsigned int PJWHash(const std::string& str)
{
    unsigned int hash = 0;
    unsigned int test = 0;
    for (char c : str)
    {
        hash = (hash << 4) + c;
        if ((test = hash & 0xF0000000) != 0)
        {
            hash ^= (test >> 24);
            hash &= ~test;
        }
    }
    return hash;
}

unsigned int ELFHash(const std::string& str)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    for (char c : str)
    {
        hash = (hash << 4) + c;
        if ((x = hash & 0xF0000000) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

unsigned int BKDRHash(const std::string& str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    for (char c : str)
    {
        hash = hash * seed + c;
    }
    return hash;
}

unsigned int SDBMHash(const std::string& str)
{
    unsigned int hash = 0;
    for (char c : str)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

unsigned int DJBHash(const std::string& str)
{
    unsigned int hash = 5381;
    for (char c : str)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned int DEKHash(const std::string& str)
{
    unsigned int hash = str.length();
    for (char c : str)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ c;
    }
    return hash;
}

unsigned int APHash(const std::string& str)
{
    unsigned int hash = 0xAAAAAAAA;
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        if ((i & 1) == 0)
            hash ^= ((hash << 7) ^ str[i] * (hash >> 3));
        else
            hash ^= (~((hash << 11) + (str[i] ^ (hash >> 5))));
    }
    return hash;
}

std::vector<std::string> generate_strings(std::size_t count)
{
    std::vector<std::string> result;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> char_dist('a', 'z');

    for (std::size_t i = 0; i < count; ++i)
    {
        std::string s(10, 'a');
        for (char& c : s)
            c = static_cast<char>(char_dist(gen));
        result.push_back(s);
    }
    return result;
}

std::size_t count_collisions(const std::vector<std::string>& data,
                             const HashFunc& hash_func,
                             unsigned int mod)
{
    std::unordered_set<unsigned int> hashes;

    for (const auto& s : data)
    {
        hashes.insert(hash_func(s) % mod);
    }

    return data.size() - hashes.size();
}

TEST(HashTest, BasicFunctionality)
{
    std::string s = "hello";
    EXPECT_NE(RSHash(s), 0u);
    EXPECT_NE(DJBHash(s), 0u);
    EXPECT_NE(SDBMHash(s), 0u);
}

TEST(HashTest, CollisionCheck)
{
    auto data = generate_strings(100);
    std::size_t collisions = count_collisions(data, DJBHash, 1000);
    EXPECT_LE(collisions, 100u);
}

TEST(HashTest, CollisionExperiment)
{
    std::vector<std::pair<std::string, HashFunc>> functions = {
        {"RS", RSHash}, {"JS", JSHash}, {"PJW", PJWHash},
        {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
        {"DJB", DJBHash}, {"DEK", DEKHash}, {"AP", APHash}
    };

    const unsigned int MOD = 1000;

    std::ofstream out("data.txt");
    if (!out.is_open())
    {
        FAIL() << "Cannot open data.txt for writing";
    }

    for (std::size_t size = 100; size <= 2000; size += 300)
    {
        auto data = generate_strings(size);

        out << size;

        for (auto& [name, func] : functions)
        {
            std::size_t collisions = count_collisions(data, func, MOD);
            out << " " << collisions;
        }

        out << "\n";
    }

    SUCCEED();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}