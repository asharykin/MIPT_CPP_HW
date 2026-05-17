#include <iostream>

class Wrapper
{
public:

    using FuncPtr = Wrapper (*)();

    Wrapper(FuncPtr ptr) : func_ptr(ptr) { }

    operator FuncPtr()
    {
        return func_ptr;
    }

private:

    FuncPtr func_ptr;
};

Wrapper test()
{
    std::cout << "test()" << std::endl;
    return test;
}

int main()
{
    Wrapper function = test();
    (*function)();
}