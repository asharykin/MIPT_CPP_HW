////////////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cmath>
#include <functional>
#include <future>
#include <iterator>
#include <numeric>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>
#include <chrono>
#include <benchmark/benchmark.h>

////////////////////////////////////////////////////////////////////////////////////////////////

template < std::ranges::view V > class Task
{
public:

  using Val = std::ranges::range_value_t<V>;

  auto operator()(V view) const
  {
    return *std::ranges::fold_left_first(view, [](long long sum, Val n) -> long long
    {
      auto nd = static_cast<double>(n);

      auto val = nd * (std::sin(nd) * std::sin(nd) + std::cos(nd) * std::cos(nd));

      return sum + std::llround(val);
    });
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T > auto fold(std::ranges::view auto view, T sum, unsigned int num_threads)
{
  auto begin = std::begin(view), end = std::end(view);

  if (auto size = std::distance(begin, end); size > 0)
  {
    auto concurrency = num_threads;

    std::vector < std::pair < std::future < T > , std::jthread > > futures(concurrency > 1 ? concurrency - 1 : 0);

    auto step = size / concurrency;

    for (auto & [future, thread] : futures)
    {
      auto range = std::ranges::subrange(begin, std::next(begin, step));

      std::packaged_task < T(decltype(range)) > task { Task < decltype(range) > () };

      future = task.get_future();

      thread = std::jthread(std::move(task), range);

      std::advance(begin, step);
    }

    auto range = std::ranges::subrange(begin, end);

    sum += Task < decltype(range) > ()(range);

    for (auto & [future, thread] : futures)
    {
      sum += future.get();
    }
  }

  return sum;
}

void BM_fold(benchmark::State& state)
{
  static std::vector<int> data(1 << 20);

  std::iota(data.begin(), data.end(), 1);

  unsigned int threads = static_cast<unsigned int>(state.range(0));

  for (auto _ : state)
  {
    long long s = fold(std::views::all(data), 0LL, threads);

    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK(BM_fold)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32);

int main(int argc, char** argv)
{
  ::benchmark::Initialize(&argc, argv);

  ::benchmark::RunSpecifiedBenchmarks();
}
