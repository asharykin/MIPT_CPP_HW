#include <gtest/gtest.h>
#include <memory>
#include <iterator>

template <typename T>
class List
{
private:

    struct Node
    {
        T x = T();
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
    };

public:

    class Iterator
    {
    public:

        using iterator_category = std::bidirectional_iterator_tag;

        Iterator(std::shared_ptr<Node> node = nullptr) : m_node(node) { }

        Iterator& operator++()
        {
            if (m_node)
            {
                m_node = m_node->next;
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--()
        {
            if (m_node)
            {
                m_node = m_node->prev.lock();
            }
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        T& operator*() const { return m_node->x; }

        T* operator->() const { return &(m_node->x); }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs.m_node == rhs.m_node;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:

        std::shared_ptr<Node> m_node;
    };

    Iterator begin() const { return Iterator(m_head); }

    Iterator end() const { return Iterator(); }

    void push_back(T x)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>(x);
        if (m_head)
        {
            node->prev = m_tail;
            m_tail->next = node;
            m_tail = node;
        }
        else
        {
            m_head = node;
            m_tail = node;
        }
    }

private:

    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
};

TEST(ListTest, EmptyList)
{
    List<int> list;
    EXPECT_EQ(std::begin(list), std::end(list));
}

TEST(ListTest, ForwardIteration)
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);

    auto it = std::begin(list);
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    EXPECT_EQ(*(it++), 2);
    EXPECT_EQ(it, std::end(list));
}

TEST(ListTest, BackwardIteration)
{
    List<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    auto it = std::begin(list);
    ++it;
    ++it;
    EXPECT_EQ(*it, 30);

    --it;
    EXPECT_EQ(*it, 20);

    EXPECT_EQ(*(it--), 20);
    EXPECT_EQ(*it, 10);

    --it;
    EXPECT_EQ(it, std::end(list));
}

TEST(ListTest, RangeBasedFor)
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    int sum = 0;
    for (auto const& element : list)
    {
        sum += element;
    }
    EXPECT_EQ(sum, 6);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
