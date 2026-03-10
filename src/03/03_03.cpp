#include <iostream>
#include <cassert>

class List
{
public:
    ~List()
    {
        while (!empty())
        {
            pop_front();
        }
    }

    bool empty() const
    {
        return m_head == nullptr;
    }

    void show() const
    {
        Node* current = m_head;
        while (current != nullptr)
        {
            std::cout << current->m_value << ' ';
            current = current->m_next;
        }
        std::cout << '\n';
    }

    void push_front(int val)
    {
        Node* node = new Node(val);
        if (empty())
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            node->m_next = m_head;
            m_head = node;
        }
    }

    void push_back(int val)
    {
        Node* node = new Node(val);
        if (empty())
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            m_tail = node;
        }
    }

    void pop_front()
    {
        if (empty())
        {
            return;
        }
        Node* tmp = m_head;
        m_head = m_head->m_next;
        delete tmp;
        if (m_head == nullptr)
        {
            m_tail = nullptr;
        }
    }

    void pop_back() {
        if (empty())
        {
            return;
        }
        if (m_head == m_tail)
        {
            delete m_head;
            m_head = m_tail = nullptr;
            return;
        }
        Node* current = m_head;
        while (current->m_next != m_tail)
        {
            current = current->m_next;
        }
        delete m_tail;
        m_tail = current;
        m_tail->m_next = nullptr;
    }

    int get() const
    {
        if (empty())
        {
            return 0;
        }
        Node* slow = m_head;
        Node* fast = m_head;
        Node* prev = nullptr;
        while (fast != nullptr && fast->m_next != nullptr)
        {
            prev = slow;
            slow = slow->m_next;
            fast = fast->m_next->m_next;
        }
        return (fast == nullptr && prev != nullptr) ? prev->m_value : slow->m_value;
    }

private:
    struct Node
    {
        int m_value;
        Node* m_next;

        Node(int val) : m_value(val), m_next(nullptr) { }
    };

    Node* m_head = nullptr;
    Node* m_tail =nullptr;
};

int main()
{
    List list;
    assert(list.empty());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    assert(!list.empty());
    assert(list.get() == 3); // middle element

    list.push_front(0);
    assert(list.get() == 2); // middle element after pushing front 0

    list.pop_front();
    assert(list.get() == 3); // middle element after popping front 0

    list.push_back(6);
    list.push_back(7);
    assert(list.get() == 4); // middle element after pushing back 6 and 7

    list.pop_back();
    assert(list.get() == 3); // middle element after popping back 7
}

