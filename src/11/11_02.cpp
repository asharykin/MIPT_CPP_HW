#include <benchmark/benchmark.h>
#include <functional>

#define NOINLINE __attribute__((noinline))

NOINLINE int free_function(int x)
{
    return x + 1;
}

class MyClass
{
public:

    NOINLINE int member_function(int x) const
    {
        return x + 1;
    }
};

class Base
{
public:

    virtual ~Base() = default;
    virtual int virtual_function(int x) = 0;
};

class Derived : public Base
{
public:

    NOINLINE int virtual_function(int x) override
    {
        return x + 1;
    }
};

class Functor
{
public:

    NOINLINE int operator()(int x) const
    {
        return x + 1;
    }
};

auto lambda_auto = [](int x) NOINLINE
{
    return x + 1;
};

std::function std_func = [](int x) NOINLINE
{
    return x + 1;
};

void BM_FreeFunction(benchmark::State& state)
{
    int x = 0;
    for (auto _ : state)
    {
        x = free_function(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_FreeFunction);

void BM_MemberFunction(benchmark::State& state)
{
    MyClass obj;
    int x = 0;
    for (auto _ : state)
    {
        x = obj.member_function(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_MemberFunction);

void BM_VirtualFunction(benchmark::State& state)
{
    Derived derived;
    Base* base_ptr = &derived;
    int x = 0;
    for (auto _ : state)
    {
        x = base_ptr->virtual_function(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_VirtualFunction);

void BM_Functor(benchmark::State& state)
{
    Functor f;
    int x = 0;
    for (auto _ : state)
    {
        x = f(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_Functor);

void BM_LambdaAuto(benchmark::State& state)
{
    int x = 0;
    for (auto _ : state)
    {
        x = lambda_auto(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_LambdaAuto);

void BM_StdFunction(benchmark::State& state)
{
    int x = 0;
    for (auto _ : state)
    {
        x = std_func(x);
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_StdFunction);

int main(int argc, char** argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}