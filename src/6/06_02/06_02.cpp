import rational;

import <cassert>;

int main()
{
    using namespace math;

    Rational x(1); // 1/1
    Rational y(2, 1); // 2/1

    assert(equals(static_cast<double>(x), 1.0));

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
