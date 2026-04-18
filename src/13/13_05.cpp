#include <iostream>
#include <string>
#include <charconv>
#include <benchmark/benchmark.h>
#include <nlohmann/json.hpp>

float custom_parse_float(const std::string& str, size_t pos, size_t len)
{
    const char* ptr = str.data() + pos;
    const char* end = ptr + len;

    float sign = 1.0f;
    if (ptr < end && *ptr == '-')
    {
        sign = -1.0f;
        ptr++;
    }

    double result = 0.0;
    while (ptr < end && *ptr >= '0' && *ptr <= '9')
    {
        result = result * 10.0 + (*ptr - '0');
        ptr++;
    }

    if (ptr < end && *ptr == '.')
    {
        ptr++;

        double fraction = 0.1;
        while (ptr < end && *ptr >= '0' && *ptr <= '9')
        {
            result += (*ptr - '0') * fraction;
            fraction /= 10.0;
            ptr++;
        }
    }

    return static_cast<float>(sign * result);
}

float std_parse_float(const std::string& str, size_t pos, size_t len)
{
    const char* first = str.data() + pos;
    const char* last = first + len;

    float value = 0.0f;
    std::from_chars(first, last, value);
    return value;
}

float nlohmann_parse_float(const std::string& str)
{
    auto j = nlohmann::json::parse(str);
    return j["value"].get<float>();
}

static void BM_CustomParse(benchmark::State& state)
{
    std::string json_data = R"({"value": -123.456, "status": "ok"})";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(custom_parse_float(json_data, 10, 8));
    }
}
BENCHMARK(BM_CustomParse);

static void BM_StdParse(benchmark::State& state)
{
    std::string json_data = R"({"value": -123.456, "status": "ok"})";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std_parse_float(json_data, 10, 8));
    }
}
BENCHMARK(BM_StdParse);

static void BM_NlohmannParse(benchmark::State& state)
{
    std::string json_data = R"({"value": -123.456, "status": "ok"})";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(nlohmann_parse_float(json_data));
    }
}
BENCHMARK(BM_NlohmannParse);

int main(int argc, char** argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
