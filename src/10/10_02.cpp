#include <iterator>
#include <stack>
#include <gtest/gtest.h>

template <typename T> class Stack
{
public:

    virtual ~Stack() = default;

    virtual void push(T x) = 0;

    virtual void pop() = 0;

    virtual auto top() const -> T = 0;

    virtual auto min() const -> T = 0;
};


template <typename T> class StackPair : public Stack<T>
{
public:

    void push(T x) override
    {
        m_stack.emplace(x, std::empty(m_stack) ? x : std::min(x, m_stack.top().second));
    }

    void pop() override
    {
        m_stack.pop();
    }

    auto top() const -> T override
    {
        return m_stack.top().first;
    }

    auto min() const -> T override
    {
        return m_stack.top().second;
    }

private:

    std::stack<std::pair<T, T>> m_stack;
};


template <typename T> class StackArithmetic : public Stack<T>
{
public:

    void push(T x) override
    {
        if (std::empty(m_stack))
        {
            m_stack.push(x);
            m_min = x;
        }
        else if (x < m_min)
        {
            m_stack.push(2 * x - m_min);
            m_min = x;
        }
        else
        {
            m_stack.push(x);
        }
    }

    void pop() override
    {
        if (auto t = m_stack.top(); t < m_min)
        {
            m_min = 2 * m_min - t;
        }
        m_stack.pop();
    }

    auto top() const -> T override
    {
        return m_stack.top() < m_min ? m_min : m_stack.top();
    }

    auto min() const -> T override
    {
        return m_min;
    }

private:

    std::stack<T> m_stack;

    T m_min = T();
};

template <typename T>
class MinStackTest : public ::testing::Test
{
protected:

    T stack;
};

using MyTypes = ::testing::Types<StackPair<int>, StackArithmetic<int>>;

TYPED_TEST_SUITE(MinStackTest, MyTypes);

TYPED_TEST(MinStackTest, SequentialOperations)
{
    this->stack.push(0);
    this->stack.push(5);
    EXPECT_EQ(this->stack.min(), 0);
    EXPECT_EQ(this->stack.top(), 5);

    this->stack.push(-3);
    EXPECT_EQ(this->stack.min(), -3);
    EXPECT_EQ(this->stack.top(), -3);

    this->stack.pop();
    EXPECT_EQ(this->stack.min(), 0);
    EXPECT_EQ(this->stack.top(), 5);
}

TYPED_TEST(MinStackTest, DuplicateMinimum)
{
    this->stack.push(-2);
    this->stack.push(-2);
    this->stack.push(5);
    EXPECT_EQ(this->stack.min(), -2);

    this->stack.pop();
    EXPECT_EQ(this->stack.min(), -2);

    this->stack.pop();
    EXPECT_EQ(this->stack.min(), -2);
}

TYPED_TEST(MinStackTest, AscendingSequence)
{
    this->stack.push(-10);
    this->stack.push(0);
    this->stack.push(30);
    EXPECT_EQ(this->stack.min(), -10);

    this->stack.pop();
    EXPECT_EQ(this->stack.min(), -10);

    this->stack.pop();
    EXPECT_EQ(this->stack.min(), -10);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
