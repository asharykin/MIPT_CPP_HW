#include <iostream>
#include <source_location>

class Tracer
{
public:

    Tracer(std::source_location location) : m_location(location)
    {
        std::cout << "[ENTER] " << m_location.function_name()
                  << " [" << m_location.file_name() << ":"
                  << m_location.line() << "]" << std::endl;
    }

    ~Tracer()
    {
        std::cout << "[EXIT] " << m_location.function_name() << std::endl;
    }

private:

    std::source_location m_location;
};

#ifdef NDEBUG
#define trace() ((void)0)
#else
#define trace() Tracer tracer_instance_##__LINE__(std::source_location::current())
#endif

void first_func()
{
    trace();
    std::cout << "first_func() is running" << std::endl;
}

void second_func(int x)
{
    trace();
    std::cout << "second_func() is running" << std::endl;
    if (x > 0)
    {
        first_func();
    }
}

int main()
{
    trace();
    std::cout << "main() is running" << std::endl;
    second_func(10);
}
