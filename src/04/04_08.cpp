#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename ... Ts> class Tuple;

template <typename T, typename ... Ts> class Tuple<T, Ts ...>
{
public:
    constexpr Tuple(T&& x, Ts&& ... xs) : m_head(std::forward<T>(x)), m_tail(std::forward<Ts>(xs)...) { }

    template <std::size_t I>
    constexpr auto get() const
    {
        if constexpr (I > 0)
        {
            return m_tail.template get<I - 1>();
        } else {
            return m_head;
        }
    }

    constexpr std::size_t size() const
    {
        return 1 + sizeof...(Ts);
    }

private:
    T m_head;
    Tuple<Ts ...> m_tail;
};

template <> class Tuple<>
{
public:
    constexpr std::size_t size() const
    {
        return 0;
    }
};


int main()
{
    static_assert(Tuple<int, double, std::string>(1, 2.0, "aaaaa").get<0>() == 1);
    static_assert(Tuple<int, double, std::string>(1, 2.0, "aaaaa").get<1>() == 2.0);
    static_assert(Tuple<int, double, std::string>(1, 2.0, "aaaaa").get<2>() == "aaaaa");

    static_assert(Tuple<>().size() == 0);
    static_assert(Tuple<int>(1).size() == 1);
    static_assert(Tuple<int, double>(1, 2.0).size() == 2);
    static_assert(Tuple<int, double, std::string>(1, 2.0, "aaaaa").size() == 3);
}
