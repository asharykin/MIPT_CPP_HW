#include <numeric>

#include "rational.hpp"
#include "custom_exception.hpp"

Rational::Rational(int num, int den) : m_num(num), m_den(den)
{
    if (den == 0)
    {
        throw Exception();
    }
    reduce();
}

void Rational::reduce()
{
    auto gcd = std::gcd(m_num, m_den);
    m_num /= gcd;
    m_den /= gcd;
}

Rational& Rational::operator+=(Rational const& other)
{
    int lcm_den = std::lcm(m_den, other.m_den);
    m_num = m_num * (lcm_den / m_den) + other.m_num * (lcm_den / other.m_den);
    m_den = lcm_den;
    reduce();
    return *this;
}

Rational& Rational::operator-=(Rational const& other)
{
    return *this += Rational(-other.m_num, other.m_den);
}

Rational& Rational::operator*=(Rational const& other)
{
    m_num *= other.m_num;
    m_den *= other.m_den;
    reduce();
    return *this;
}

Rational& Rational::operator/=(Rational const& other)
{
    return *this *= Rational(other.m_den, other.m_num);
}

Rational Rational::operator++(int)
{
    Rational copy = *this;
    ++(*this);
    return copy;
}

Rational Rational::operator--(int)
{
    Rational copy = *this;
    --(*this);
    return copy;
}

std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs)
{
    return lhs.m_num * rhs.m_den <=> rhs.m_num * lhs.m_den;
}

bool operator==(Rational const& lhs, Rational const& rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::equal;
}

std::istream& operator>>(std::istream& stream, Rational& rational)
{
    return (stream >> rational.m_num).ignore() >> rational.m_den;
}

std::ostream& operator<<(std::ostream& stream, Rational const& rational)
{
    return stream << rational.m_num << '/' << rational.m_den;
}
