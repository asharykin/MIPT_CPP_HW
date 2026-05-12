#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <new>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <gtest/gtest.h>

using namespace std::literals;

template <typename T>
class Storage
{
public:

    Storage(std::size_t size) : m_size(size), m_mask(size - 1)
    {
        assert((m_size & m_mask) == 0);

        m_array = static_cast <T*>(operator new(sizeof(T) * m_size, std::align_val_t(s_alignment)));

        for (auto i = 0uz; i < m_size; ++i)
        {
            std::construct_at(&m_array[i], T());
        }
    }

    ~Storage()
    {
        std::destroy(m_array, m_array + m_size);

        operator delete(m_array, std::align_val_t(s_alignment));
    }

    auto       & operator[](std::size_t index)       { return m_array[index & m_mask]; }

    auto const & operator[](std::size_t index) const { return m_array[index & m_mask]; }

private:

    T * m_array = nullptr;

    std::size_t m_size = 0, m_mask = 0;

    static inline auto s_alignment = std::hardware_destructive_interference_size;
};

using index_t = std::int64_t;

class alignas(std::hardware_destructive_interference_size) Index
{
public:

    auto get(std::memory_order order) const { return m_index.load(order); }

    void set(index_t index, std::memory_order order) { m_index.store(index, order); }

private:

    std::atomic < index_t > m_index = -1;

    std::byte m_padding[std::hardware_destructive_interference_size - sizeof(std::atomic < index_t > )];
};

struct BusyWait
{
    void wait() { __builtin_ia32_pause(); }
};

struct YieldWait
{
    void wait() { std::this_thread::yield(); }
};

struct BlockingWait
{
    void wait()
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait_for(lock, 1ms);
    }

    void notify() { m_cv.notify_all(); }

private:

    std::mutex m_mutex;

    std::condition_variable m_cv;
};

template <typename Strategy>
class Barrier
{
public:

    Barrier(Index& index, Strategy& strategy) : m_index(index), m_strategy(strategy) { }

    void wait_until(index_t target)
    {
        while (m_index.get(std::memory_order::acquire) < target)
        {
            m_strategy.wait();
        }
    }

private:

    Index& m_index;

    Strategy& m_strategy;
};

class Controller
{
public:

    Controller(Index & producer_index, Index const & consumer_index, index_t size)
    : m_producer_index(producer_index), m_consumer_index(consumer_index), m_size(size) { }

    template <typename Strategy>
    auto get(Barrier<Strategy>& barrier, index_t target) const
    {
        auto i = m_producer_index.get(std::memory_order::relaxed) + 1;

        if (auto j = i - m_size; j > m_cached_consumer_index)
        {
            barrier.wait_until(j);

            m_cached_consumer_index = m_consumer_index.get(std::memory_order::acquire);
        }

        return i;
    }

    void set(index_t index) const { m_producer_index.set(index, std::memory_order::release); }

private:

    Index& m_producer_index;

    Index const& m_consumer_index;

    index_t m_size = 0;

    mutable index_t m_cached_consumer_index = -1;
};

template <typename Strategy>
void produce(Storage < int > & storage, Controller const & controller, Barrier<Strategy>& barrier)
{
    std::this_thread::sleep_for(1s);

    for (auto i = 1; i < (1 << 10) + 1; ++i)
    {
        auto j = controller.get(barrier, 0);

        storage[j] = i;

        controller.set(j);
    }
}

template <typename Strategy>
void consume(Storage < int > const & storage, Index const & producer_index, Index & consumer_index, Barrier<Strategy>& barrier)
{
    index_t i = 0;

    while (i < 1 << 10)
    {
        barrier.wait_until(i);

        index_t j = producer_index.get(std::memory_order::acquire);

        while (i <= j)
        {
            auto x = storage[i++];

            std::cout << std::format("consume : x = {}\n", x);
        }

        consumer_index.set(i - 1, std::memory_order::release);
    }
}

template <typename Strategy>
void RunDisruptorTest()
{
    auto size = 1 << 10;

    Storage < int > storage(size);

    Index producer_index, consumer_index;

    Controller controller(producer_index, consumer_index, size);

    Strategy prod_strat, cons_strat;

    Barrier<Strategy> prod_barrier(consumer_index, prod_strat);

    Barrier<Strategy> cons_barrier(producer_index, cons_strat);

    std::jthread t1(produce<Strategy>, std::ref(storage), std::ref(controller), std::ref(prod_barrier));

    std::jthread t2(consume<Strategy>, std::ref(storage), std::ref(producer_index), std::ref(consumer_index), std::ref(cons_barrier));
}

TEST(DisruptorTest, BusyWaitStrategy) { RunDisruptorTest<BusyWait>(); }

TEST(DisruptorTest, YieldWaitStrategy) { RunDisruptorTest<YieldWait>(); }

TEST(DisruptorTest, BlockingWaitStrategy) { RunDisruptorTest<BlockingWait>(); }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}