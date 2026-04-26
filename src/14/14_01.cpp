#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>
#include <functional>

void func(std::exception_ptr& eptr)
{
    try
    {
        throw std::runtime_error("Exception from thread1");
    }
    catch (...)
    {
        eptr = std::current_exception();
    }
}

int main() {
    std::exception_ptr eptr = nullptr;

    std::thread thread1(func, std::ref(eptr));
    thread1.join();

    if (eptr)
    {
        try
        {
            std::rethrow_exception(eptr);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
    }
}