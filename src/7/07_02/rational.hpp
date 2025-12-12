#pragma once

#include <compare>
#include <iostream>

class Rational
{
public:
    Rational(int num = 0, int den = 1);

    explicit operator double() const { return static_cast<double>(m_num) / m_den; }

    void reduce();

    Rational& operator+=(Rational const& other);
    Rational& operator-=(Rational const& other);
    Rational& operator*=(Rational const& other);
    Rational& operator/=(Rational const& other);

    Rational& operator++() { return *this += Rational(1); }
    Rational& operator--() { return *this -= Rational(1); }

    Rational operator++(int);
    Rational operator--(int);

    friend Rational operator+(Rational lhs, Rational const& rhs) { return lhs += rhs; }
    friend Rational operator-(Rational lhs, Rational const& rhs) {return lhs -= rhs; }
    friend Rational operator*(Rational lhs, Rational const& rhs) { return lhs *= rhs; }
    friend Rational operator/(Rational lhs, Rational const& rhs) { return lhs /= rhs; }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs);
    friend bool operator==(Rational const& lhs, Rational const& rhs);

    friend std::istream& operator>>(std::istream& stream, Rational& rational);
    friend std::ostream& operator<<(std::ostream& stream, Rational const& rational);

private:
    int m_num;
    int m_den;
};
