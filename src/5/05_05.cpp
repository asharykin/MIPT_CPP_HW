#include <cassert>
#include <iostream>
#include <numeric>
#include <compare>
#include <sstream>

template <typename T> class addable
{
public:
    friend T operator+(T lhs, T const& rhs) { return lhs += rhs; }
};

template <typename T> class subtractable
{
public:
    friend T operator-(T lhs, T const& rhs) { return lhs -= rhs; }
};

template <typename T> class multipliable
{
public:
    friend T operator*(T lhs, T const& rhs) { return lhs *= rhs; }
};

template <typename T> class dividable
{
public:
    friend T operator/(T lhs, T const& rhs) { return lhs /= rhs; }
};

template <typename T> class incrementable
{
public:
    friend T operator++(T& obj) { return obj += T(1); }

    friend T operator++(T& obj, int)
    {
        T copy = obj;
        ++obj;
        return copy;
    }
};

template <typename T> class decrementable
{
public:
    friend T operator--(T& obj) { return obj -= T(1); }

    friend T operator--(T& obj, int)
    {
        T copy = obj;
        --obj;
        return copy;
    }
};

class Rational : public addable<Rational>, public subtractable<Rational>,
                 public multipliable<Rational>, public dividable<Rational>,
                 public incrementable<Rational>, public decrementable<Rational>
{
public:
    Rational(int num = 0, int den = 1) : m_num(num), m_den(den)
    {
        reduce();
    }

    explicit operator double() const
    {
        return static_cast<double>(m_num) / m_den;
    }

    Rational& operator+=(Rational const& other)
    {
        int lcm_den = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm_den / m_den) + other.m_num * (lcm_den / other.m_den);
        m_den = lcm_den;
        reduce();
        return *this;
    }

    Rational& operator-=(Rational const& other)
    {
        return *this += Rational(-other.m_num, other.m_den);
    }

    Rational& operator*=(Rational const& other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    Rational& operator/=(Rational const& other)
    {
        return *this *= Rational(other.m_den, other.m_num);
    }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs)
    {
        return lhs.m_num * rhs.m_den <=> rhs.m_num * lhs.m_den;
    }

    friend bool operator==(Rational const& lhs, Rational const& rhs)
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend std::istream& operator>>(std::istream& stream, Rational& rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend std::ostream& operator<<(std::ostream& stream, Rational const& rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    int m_num;
    int m_den;

    void reduce()
    {
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }
};

bool equal(double x, double y, double epsilon = 0.00001)
{
    return std::abs(x - y) < epsilon;
}


int main()
{
    Rational x(1); // 1/1
    Rational y(2, 1); // 2/1

    assert(equal(static_cast<double>(x), 1.0));

    assert((x += y) == Rational(3, 1));
    assert((x -= y) == Rational(1, 1));
    assert((x *= y) == Rational(2, 1));
    assert((x /= y) == Rational(1, 1));

    assert((x++) == Rational(1, 1));
    assert((x--) == Rational(2, 1));
    assert((++y) == Rational(3, 1));
    assert((--y) == Rational(2, 1));

    assert((x + y) == Rational(3, 1));
    assert((x - y) == Rational(-1, 1));
    assert((x * y) == Rational(2, 1));
    assert((x / y) == Rational(1, 2));

    assert((x += 1) == Rational(2, 1));
    assert((x + 1) == Rational(3, 1));
    assert((1 + y) == Rational(3, 1));
    assert((1 + 1) == Rational(2, 1));

    assert((x < y) == 0);
    assert((x > y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 1);
    assert((x == y) == 1);
    assert((x != y) == 0);

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;
    stream_1 >> x;
    stream_2 << x;
    assert(stream_2.str() == stream_1.str());
}
