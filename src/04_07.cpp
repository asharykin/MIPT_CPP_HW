#include <cassert>
#include <numeric>

template <int N = 0, int D = 1> struct Ratio
{
    static_assert(D != 0, "Denominator cannot be zero");
    constexpr static auto num = N;
    constexpr static auto den = D;

    using simplified = Ratio<N / std::gcd(N, D), D / std::gcd(N, D)>;
};

template <typename R1, typename R2> struct Sum
{
    constexpr static auto num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto den = R1::den * R2::den;

    using type = typename Ratio<num, den>::simplified;
};

template <typename R1, typename R2> using sum = typename Sum<R1, R2>::type;

template <typename R1, typename R2> struct Sub
{
    using type = sum<R1, Ratio<-R2::num, R2::den>>;
};

template <typename R1, typename R2> using sub = typename Sub<R1, R2>::type;

template <typename R1, typename R2> struct Mul
{
    constexpr static auto num = R1::num * R2::num;
    constexpr static auto den = R1::den * R2::den;

    using type = typename Ratio<num, den>::simplified;
};

template <typename R1, typename R2> using mul = typename Mul<R1, R2>::type;

template <typename R1, typename R2> struct Div
{
    static_assert(R2::num != 0, "Cannot divide by zero");

    using type = mul<R1, Ratio<R2::den, R2::num>>;
};

template <typename R1, typename R2> using divi = typename Div<R1, R2>::type;

template <typename T, typename R = Ratio<1>> struct Duration
{
    T x = T();
};

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    using ratio_t = Ratio<1, sum<R1, R2>::den>;
    auto x = (lhs.x * ratio_t::den / R1::den * R1::num +
               rhs.x * ratio_t::den / R2::den * R2::num);
    return Duration<decltype(x), ratio_t>(x);
}

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    return lhs + Duration<T2, Ratio<-R2::num, R2::den>>(rhs.x);
}

int main() {
    using R1 = Ratio<1, 2>;
    using R2 = Ratio<1, 3>;

    using R3 = sum<R1, R2>; // 1/2 + 1/3 = 5/6
    static_assert(R3::num == 5);
    static_assert(R3::den == 6);

    using R4 = sub<R1, R2>; // 1/2 - 1/3 = 1/6
    static_assert(R4::num == 1);
    static_assert(R4::den == 6);

    using R5 = Ratio<2, 3>;
    using R6 = Ratio<3, 4>;

    using R7 = mul<R5, R6>; // 2/3 * 3/4 = 1/2
    static_assert(R7::num == 1);
    static_assert(R7::den == 2);

    using R8 = divi<R5, R6>; // 2/3 / 3/4
    static_assert(R8::num == 8);
    static_assert(R8::den == 9);

    constexpr Duration<int, R1> duration_1(1);
    constexpr Duration<int, R2> duration_2(2);
    constexpr Duration <int, Ratio <1, 6>> duration_3 = duration_1 + duration_2;
    static_assert(duration_3.x == 7);

    constexpr Duration<int, Ratio<1, 6>> duration_4 = duration_1 - duration_2;
    static_assert(duration_4.x == -1);
}