#pragma once

struct NoCopyable
{
    NoCopyable() = default;
    NoCopyable(const NoCopyable& other) = delete;
    virtual ~NoCopyable()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    };
};