#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

template<typename T> class Vector
{
public:
    Vector() : m_array(nullptr), m_size(0), m_capacity(1)
    {
        m_array = new T[m_capacity] {};
    }

    Vector(std::initializer_list<T> list) : m_size(list.size()), m_capacity(list.size())
    {
        m_array = new T[m_capacity] {};
        std::ranges::copy(list, m_array);
    }

    Vector(Vector const& other) : m_size(other.m_size), m_capacity(other.m_capacity)
    {
        m_array = m_size ? new T[m_capacity] {} : nullptr;
        std::ranges::copy(other.m_array, other.m_array + m_size, m_array);
    }

    Vector(Vector&& other)
        : m_array(std::exchange(other.m_array, nullptr)),
          m_size(std::exchange(other.m_size, 0)),
          m_capacity(std::exchange(other.m_capacity, 0))
    { }

    ~Vector()
    {
        delete[] m_array;
    }

    Vector& operator=(Vector other)
    {
        swap(other);
        return *this;
    }

    void swap(Vector& other)
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    std::size_t size() const { return m_size; }

    std::size_t capacity() const { return m_capacity; }

    void push_back(T value)
    {
        if (m_size >= m_capacity)
        {
            std::size_t new_capacity = m_capacity * 2;
            T* new_array = new T[new_capacity] {};
            std::ranges::copy(m_array, m_array + m_size, new_array);
            delete[] m_array;
            m_array = new_array;
            m_capacity = new_capacity;
        }
        m_array[m_size++] = value;
    }

    void clear() {
        m_size = 0;
    }

    bool empty() const {
        return m_size == 0;
    }

    T& operator[](std::size_t index) { return m_array[index]; }

    const T& operator[](std::size_t index) const { return m_array[index]; }

private:
    T* m_array;
    std::size_t m_size;
    std::size_t m_capacity;
};

int main() {
    Vector<int> v = {1, 2, 3};
    v.push_back(4);
    v.push_back(5);

    assert(v.size() == 5);
    assert(v.capacity() == 6);
    assert(!v.empty());
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[v.size() - 1] == 5);

    v.clear();
    assert(v.size() == 0);
    assert(v.empty());
}
