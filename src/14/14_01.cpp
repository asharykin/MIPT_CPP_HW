#include <gtest/gtest.h>
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

TEST(ThreadExceptionTest, CaptureAndRethrowRuntimeError)
{
    std::exception_ptr exptr = nullptr;

    std::thread t(func, std::ref(exptr));
    t.join();

    ASSERT_NE(exptr, nullptr);

    bool caught_ex = false;

    try
    {
        std::rethrow_exception(exptr);
    }
    catch (const std::runtime_error& ex)
    {
        caught_ex = true;
        ASSERT_STREQ(ex.what(), "Exception from thread1");
    }

    ASSERT_TRUE(caught_ex);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}