#include <gtest/gtest.h>

class Iterator
{
public:

    Iterator() : m_previous(0), m_current(1) { }

    int operator*() const
    {
        return m_current; // latest computed value
    }

    Iterator& operator++()
    {
        int temp = m_previous + m_current;
        m_previous = m_current;
        m_current = temp;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const Iterator &other) const
    {
        return m_previous == other.m_previous && m_current == other.m_current;
    }

    bool operator!=(const Iterator &other) const
    {
        return !(*this == other);
    }

private:

    int m_previous;
    int m_current;
};

class FibonacciTest : public ::testing::Test
{
protected:
    Iterator it;
};


TEST_F(FibonacciTest, AdvancesCorrectly)
{
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 5);
}

TEST_F(FibonacciTest, PostfixReturnsOldValue)
{
    ++it;
    Iterator old = it++;
    EXPECT_EQ(*old, 1);
    EXPECT_EQ(*it, 2);
}

TEST_F(FibonacciTest, EqualityComparison)
{
    Iterator other;
    EXPECT_EQ(it, other);
    ++it;
    EXPECT_NE(it, other);
    ++other;
    EXPECT_EQ(it, other);
}

TEST_F(FibonacciTest, ReachesTenthNumber)
{
    for (int i = 0; i < 10; ++i)
    {
        it++;
    }
    EXPECT_EQ(*it, 89);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}