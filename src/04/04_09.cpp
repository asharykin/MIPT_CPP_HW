#include <string>
#include <type_traits>

template <typename T> class is_class
{
private :
    template <typename U> static std::false_type test(...);

    // A pointer-to-member type (int U::*) can only be formed if U is a class/struct/union.
    // This is true even for empty classes/structs (struct Empty {}). Compiler understands int Empty::*
    // as a type that could point to an int member if one existed, even if it doesn't.
    // The validity of the type itself is what matters for SFINAE, not the actual presence of such a member.
    // If U is not a class/struct/union, int U::* is ill-formed, triggering SFINAE and discarding this overload.
    template <typename U> static std::true_type test
    (
        int,
        decltype(static_cast<int U::*>(nullptr)) = nullptr
    );

public :

    constexpr static auto value = decltype(test<T>(0))::value;
};

template <typename T>
constexpr auto is_class_v = is_class<T>::value;



template<typename T> struct add_const
{
    using type = const T;
};

template<typename T>
using add_const_t = typename add_const<T>::type;



template<typename T> struct remove_const
{
    using type = T;
};

template<typename T> struct remove_const<T const>
{
    using type = T;
};

template<typename T>
using remove_const_t = typename remove_const<T>::type;



template<bool Cond, typename IfTrue, typename IfFalse> struct conditional
{
    using type = IfTrue;
};

template<typename IfTrue, typename IfFalse> struct conditional<false, IfTrue, IfFalse>
{
    using type = IfFalse;
};

template<bool Cond, typename IfTrue, typename IfFalse>
using conditional_t = typename conditional<Cond, IfTrue, IfFalse>::type;



template<typename U>
struct decay_selector
  : conditional_t<std::is_const<const U>::value, // false for functions
            remove_const<U>,
            std::add_pointer<U>> // function decays to pointer
  { };

template<typename U, size_t N> struct decay_selector<U[N]>
{
    using type = U*;
};

template<typename U> struct decay_selector<U[]>
{
    using type = U*;
};


template<typename T> struct decay
{
    using type = typename decay_selector<T>::type;
};

template<typename T> struct decay<T&>
{
    using type = typename decay_selector<T>::type;
};

template<typename T> struct decay<T&&>
{
    using type = typename decay_selector<T>::type;
};

template<typename T>
using decay_t = typename decay<T>::type;



int main()
{
    struct My_Struct {};
    class My_Class {};
    static_assert(is_class_v<double> == false);
    static_assert(is_class_v<std::string> == true);
    static_assert(is_class_v<My_Struct> == true);
    static_assert(is_class_v<My_Struct*> == false);
    static_assert(is_class_v<My_Class> == true);
    static_assert(is_class_v<My_Class&> == false);

    static_assert(std::is_same_v<add_const_t<int>, const int>);
    static_assert(std::is_same_v<remove_const_t<const int>, int>);

    static_assert(std::is_same_v<conditional_t<sizeof(long) >= sizeof(int), long, int>, long>);
    static_assert(std::is_same_v<conditional_t<sizeof(char) >= sizeof(int), char, int>, int>);

    static_assert(std::is_same_v<decay_t<int&>, int>);
    static_assert(std::is_same_v<decay_t<int&&>, int>);
    static_assert(std::is_same_v<decay_t<int[]>, int*>);
}
