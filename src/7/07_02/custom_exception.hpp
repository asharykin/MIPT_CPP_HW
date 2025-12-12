#pragma once

#include <exception>

class Exception : public std::exception
{
public :

    Exception() = default;

    char const* what() const noexcept override
    {
        return "denominator cannot be zero!";
    }
};

