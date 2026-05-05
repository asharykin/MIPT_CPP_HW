#include <vector>
#include <thread>
#include <memory>
#include <gtest/gtest.h>

template<typename T>
class LockFreeStack
{
public:

    LockFreeStack() = default;

    ~LockFreeStack()
    {
        while(pop());
    }

    void push(T const& data)
    {
        std::shared_ptr<Node> new_node = std::make_shared<Node>(data);

        std::shared_ptr<Node> old_head = m_head.load();

        new_node->m_next.store(old_head);

        while (!m_head.compare_exchange_weak(old_head, new_node))
        {
            new_node->m_next.store(old_head);
        }
    }

    std::shared_ptr<T> pop()
    {
        std::shared_ptr<Node> old_head = m_head.load();

        while (old_head && !m_head.compare_exchange_weak(old_head, old_head->m_next.load()));

        if (old_head)
        {
            return old_head->m_data;
        }

        return std::shared_ptr<T>();
    }

private:

    struct Node
    {
        std::shared_ptr<T> m_data;

        std::atomic<std::shared_ptr<Node>> m_next;

        Node(T const& data) : m_data(std::make_shared<T>(data)) { }
    };

    std::atomic<std::shared_ptr<Node>> m_head;
};

class LockFreeStackTest : public ::testing::Test
{
protected:

    LockFreeStack<int> stack;
};

TEST_F(LockFreeStackTest, MultithreadedStressTest)
{
    const int num_threads = 4;
    const int items_per_thread = 10000;

    std::atomic<long long> total_pushed_sum{0};
    std::atomic<long long> total_popped_sum{0};
    std::atomic<int> items_popped_count{0};

    auto producer = [&](int id)
    {
        for (int i = 0; i < items_per_thread; ++i)
        {
            int val = id * items_per_thread + i;
            stack.push(val);
            total_pushed_sum += val;
        }
    };

    auto consumer = [&]()
    {
        for (int i = 0; i < items_per_thread; ++i)
        {
            std::shared_ptr<int> val;
            while (!((val = stack.pop())))
            {
                std::this_thread::yield();
            }
            total_popped_sum += *val;
            items_popped_count++;
        }
    };

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(producer, i);
        threads.emplace_back(consumer);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    EXPECT_EQ(items_popped_count.load(), num_threads * items_per_thread);
    EXPECT_EQ(total_pushed_sum.load(), total_popped_sum.load());
    EXPECT_EQ(stack.pop(), nullptr);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
