export module rational;

import <iostream>;
import <sstream>;
import <compare>;
import <numeric>;
import <cmath>;
import <cassert>;

export namespace math {

    class Rational
    {
    public:
        Rational(int num = 0, int den = 1);

        explicit operator double() const;

        Rational& operator+=(Rational const& other);
        Rational& operator-=(Rational const& other);
        Rational& operator*=(Rational const& other);
        Rational& operator/=(Rational const& other);

        Rational& operator++();
        Rational& operator--();
        Rational operator++(int);
        Rational operator--(int);

        friend Rational operator+(Rational lhs, Rational const& rhs);
        friend Rational operator-(Rational lhs, Rational const& rhs);
        friend Rational operator*(Rational lhs, Rational const& rhs);
        friend Rational operator/(Rational lhs, Rational const& rhs);

        friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs);
        friend bool operator==(Rational const& lhs, Rational const& rhs);

        friend std::istream& operator>>(std::istream& stream, Rational& rational);
        friend std::ostream& operator<<(std::ostream& stream, Rational const& rational);

    private:
        int m_num;
        int m_den;

        void reduce();
    };

    bool equals(double x, double y, double epsilon = 0.00001);
}