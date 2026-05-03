#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <deque>
#include <format>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <syncstream>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

class Callable_Base
{
public:

    virtual ~Callable_Base() = default;

    virtual void call() = 0;
};


template<typename F>
class Callable : public Callable_Base
{
public:

    Callable(F&& f) : m_f(std::forward<F>(f)) { }

    void call() override
    {
        m_f();
    }

private:

    F m_f;
};

class Task
{
public:

    Task() = default;

    Task(Task const&) = delete;

    Task& operator=(Task const&) = delete;

    template<typename F>
    Task(F&& f) : m_callable(std::make_unique< Callable<F> >( std::forward<F>(f)) ) { }

    Task(Task&& other) : m_callable(std::move(other.m_callable)) { }

    auto & operator=(Task&& other)
    {
        m_callable = std::move(other.m_callable);

        return *this;
    }

    void operator()() const
    {
        m_callable->call();
    }

private:

    std::unique_ptr<Callable_Base> m_callable;
};

class WorkStealingQueue
{
public:

    void push(Task task)
    {
        std::scoped_lock lock(m_mutex);

        m_queue.push_back(std::move(task));
    }

    bool try_pop(Task& task)
    {
        std::scoped_lock lock(m_mutex);

        if (m_queue.empty())
        {
            return false;
        }

        task = std::move(m_queue.back());

        m_queue.pop_back();

        return true;
    }

    bool try_steal(Task& task)
    {
        std::scoped_lock lock(m_mutex);

        if (m_queue.empty())
        {
            return false;
        }

        task = std::move(m_queue.front());

        m_queue.pop_front();

        return true;
    }

private:

    std::deque<Task> m_queue;

    mutable std::mutex m_mutex;
};

class Pool
{
public:

    Pool()
    {
        unsigned num_threads = std::thread::hardware_concurrency();

        for (unsigned i = 0; i < num_threads; ++i)
        {
            m_queues.push_back(std::make_unique<WorkStealingQueue>());
        }

        for (unsigned i = 0; i < num_threads; ++i)
        {
            m_threads.emplace_back(&Pool::run, this, i);
        }
    }

    ~Pool()
    {
        m_done = true;
    }

    template<typename F>
    auto post(F&& f)
    {
        std::packaged_task<std::result_of_t<F()>()> task(std::forward<F>(f));

        auto res = task.get_future();

        if (local_queue)
        {
            local_queue->push(std::move(task));
        }
        else
        {
            m_queues[0]->push(std::move(task));
        }

        return res;
    }

private:

    void run(unsigned index)
    {
        local_index = index;

        local_queue = m_queues[index].get();

        while (!m_done)
        {
            Task task;

            if (local_queue->try_pop(task) || steal_task(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    bool steal_task(Task& task)
    {
        for (unsigned i = 0; i < m_queues.size(); ++i)
        {
            unsigned target = (local_index + i + 1) % m_queues.size();

            if (m_queues[target]->try_steal(task))
            {
                return true;
            }
        }
        return false;
    }

    static inline thread_local WorkStealingQueue* local_queue = nullptr;

    static inline thread_local unsigned local_index = 0;

    std::vector<std::unique_ptr<WorkStealingQueue>> m_queues;

    std::vector<std::jthread> m_threads;

    std::atomic<bool> m_done{false};
};

auto calculate(std::size_t size)
{
    auto id = std::this_thread::get_id();

    std::osyncstream(std::cout) << std::format("calculate : id = {}\n", id);

    auto x = 0.0;

    for (auto i = 0uz; i < size; ++i)
    {
        x += std::pow(std::sin(x), 2) + std::pow(std::cos(x), 2);
    }

    return x;
}

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

TEST(PoolTest, CorrectResult)
{
    Pool pool;

    const size_t num_tasks = 100;

    const size_t calc_size = 1024;

    std::vector<std::future<double>> futures(num_tasks);

    for (auto& future : futures)
    {
        future = pool.post([calc_size]() { return calculate(calc_size); });
    }

    for (auto& future : futures)
    {
        EXPECT_TRUE(equal(future.get(), static_cast<double>(calc_size)));
    }
}

TEST(PoolTest, ManyTasks)
{
    Pool pool;

    const size_t num_tasks = 2000;

    std::vector<std::future<double>> futures(num_tasks);

    for (auto& future : futures)
    {
        future = pool.post([]() { return calculate(100); });
    }

    for (auto& future : futures)
    {
        EXPECT_NO_THROW(future.get());
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}