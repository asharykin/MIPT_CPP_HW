#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <random>
#include <unordered_set>
#include <functional>
#include <fstream>

uint32_t process_rs(const std::string& input)
{
    uint32_t magic_b = 378551;
    uint32_t magic_a = 63689;
    uint32_t result = 0;

    for (char sym : input)
    {
        result = result * magic_a + static_cast<uint32_t>(sym);
        magic_a *= magic_b;
    }

    return result;
}

uint32_t process_js(const std::string& input)
{
    uint32_t acc = 1315423911;

    for (char sym : input)
    {
        acc ^= ((acc << 5) + sym + (acc >> 2));
    }

    return acc;
}

uint32_t process_pjw(const std::string& input)
{
    uint32_t val = 0;
    uint32_t high_bits = 0;

    for (char sym : input)
    {
        val = (val << 4) + sym;

        if ((high_bits = val & 0xF0000000) != 0)
        {
            val ^= (high_bits >> 24);
            val &= ~high_bits;
        }
    }

    return val;
}

uint32_t process_elf(const std::string& input)
{
    uint32_t state = 0;
    uint32_t mask = 0;

    for (char sym : input)
    {
        state = (state << 4) + sym;
        if ((mask = state & 0xF0000000) != 0)
        {
            state ^= (mask >> 24);
        }
        state &= ~mask;
    }

    return state;
}

uint32_t process_bkdr(const std::string& input)
{
    uint32_t multiplier = 131;
    uint32_t res = 0;

    for (char sym : input)
    {
        res = res * multiplier + sym;
    }

    return res;
}

uint32_t process_sdbm(const std::string& input)
{
    uint32_t hash_val = 0;

    for (char sym : input)
    {
        hash_val = sym + (hash_val << 6) + (hash_val << 16) - hash_val;
    }

    return hash_val;
}

uint32_t process_djb(const std::string& input)
{
    uint32_t current = 5381;

    for (char sym : input)
    {
        current = ((current << 5) + current) + sym;
    }

    return current;
}

uint32_t process_dek(const std::string& input)
{
    uint32_t final_val = static_cast<uint32_t>(input.length());

    for (char sym : input)
    {
        final_val = ((final_val << 5) ^ (final_val >> 27)) ^ sym;
    }

    return final_val;
}

uint32_t process_ap(const std::string& input)
{
    uint32_t h = 0xAAAAAAAA;

    for (size_t idx = 0; idx < input.size(); ++idx)
    {
        if ((idx & 1) == 0)
        {
            h ^= ((h << 7) ^ input[idx] * (h >> 3));
        }
        else
        {
            h ^= (~((h << 11) + (input[idx] ^ (h >> 5))));
        }
    }

    return h;
}

std::vector<std::string> spawn_mock_strings(size_t total, size_t length = 12)
{
    std::vector<std::string> collection{};
    std::random_device rd{};
    std::mt19937 engine{ rd() };
    std::uniform_int_distribution<int> range('A', 'Z');

    collection.reserve(total);
    for (size_t i = 0; i < total; ++i)
    {
        std::string buffer;
        for (size_t j = 0; j < length; ++j)
        {
            buffer += static_cast<char>(range(engine));
        }
        collection.emplace_back(std::move(buffer));
    }

    return collection;
}

using ChecksumLogic = std::function<uint32_t(const std::string&)>;

size_t analyze_collision_rate(const std::vector<std::string>& payload,
                              const ChecksumLogic& algo,
                              uint32_t bucket_size) {
    std::unordered_set<uint32_t> unique_slots;
    for (const auto& item : payload) {
        unique_slots.insert(algo(item) % bucket_size);
    }
    return payload.size() - unique_slots.size();
}


TEST(AlgorithmAudit, SanityCheck) {
    const std::string sample = "test_vector";
    ASSERT_GT(process_rs(sample), 0u);
    ASSERT_GT(process_djb(sample), 0u);
    ASSERT_GT(process_sdbm(sample), 0u);
}

TEST(AlgorithmAudit, FullBenchmarkSuite) {
    struct AlgoEntry {
        std::string label;
        ChecksumLogic procedure;
    };

    std::vector<AlgoEntry> registry = {
        {"RS_ALGO", process_rs},   {"JS_ALGO", process_js},
        {"PJW_ALGO", process_pjw}, {"ELF_ALGO", process_elf},
        {"BKDR_ALGO", process_bkdr}, {"SDBM_ALGO", process_sdbm},
        {"DJB_ALGO", process_djb}, {"DEK_ALGO", process_dek},
        {"AP_ALGO", process_ap}
    };

    const uint32_t HASH_SPACE = 1024;
    std::ofstream report("benchmark_results.csv");

    if (!report.is_open()) {
        FAIL() << "Failed to initialize output stream.";
    }

    report << "Sample_Size";
    for (const auto& entry : registry) report << "," << entry.label;
    report << "\n";

    for (size_t current_batch = 150; current_batch <= 2500; current_batch += 400) {
        auto test_data = spawn_mock_strings(current_batch);
        report << current_batch;

        for (auto& item : registry) {
            size_t conflicts = analyze_collision_rate(test_data, item.procedure, HASH_SPACE);
            report << "," << conflicts;
        }
        report << "\n";
    }
    report.close();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}