#include <vector>
#include <map>
#include "hw_allocator.cpp"
#include "hw_list.cpp"

int Fibonacci(const size_t N)
{
    if (N <= 2)
        return 1; // первые 2 числа равны 1
    return static_cast<int>(Fibonacci(N - 1) + Fibonacci(N - 2));
}

int Factorial(size_t value)
{
    if (value == 0) {
        return 0;
    }
    int res = 1;
    while (value > 0) {
        res *= value--;
    }
    return res;
}

int main(int, char const *[])
{
    if(false)
    {
        std::cout << "-- Map with simple allocator -- " << std::endl;
        auto m = std::map<int, int, std::less<int>, SimpleAllocator<std::pair<const int, int>>>{};
        for (size_t i = 0; i < 5; ++i) {
            m.emplace(i, i);
            std::cout << "---" << std::endl;
        }
        for (auto[key, value]: m) {
            std::cout << key << " " << value << std::endl;
        }
    }

    if(false)
    {
        std::cout << "\n-- Map with homework allocator -- " << std::endl;
        auto m2 = std::map<int, int, std::less<int>, HwAllocator<std::pair<const int, int>, 5>>{};
        for (size_t i = 0; i < 5; ++i) {
            m2.emplace(i, i);
            std::cout << "---" << std::endl;
        }
        for (auto[key, value]: m2) {
            std::cout << key << " " << value << std::endl;
        }
    }

    {
        std::cout << "\n-- Map with homework allocator and container -- " << std::endl;
        auto m = std::map<int, Hard, std::less<int>, HwAllocator<std::pair<const int, Hard>, 10>>{};
        for (size_t i = 0; i < 11; ++i) {
            m.emplace(std::piecewise_construct,
                      std::forward_as_tuple(i),
                      std::forward_as_tuple(Factorial(i), Fibonacci(i)));
            std::cout << "---" << std::endl;
        }
        for (auto&& [key, value]: m) {
            std::cout << key << " " << value.fa << " " << value.fi   << std::endl;
        }
    }

    {
        std::cout << "\n-- Custom container with homework container and default allocator -- " << std::endl;
        auto m = HwList<Hard>{};
        for (size_t i = 0; i < 11; ++i) {
            m.EmplaceBack(Factorial(i), Fibonacci(i));
            std::cout << "---" << std::endl;
        }
        for (auto&& item : m) {
            std::cout << item.fa << " " << item.fi   << std::endl;
        }
    }

    {
        std::cout << "\n-- Custom container with homework allocator and container -- " << std::endl;
        auto m = HwList<Hard, HwAllocator<Hard, 10>>{};
        for (size_t i = 0; i < 11; ++i) {
            m.EmplaceBack(Factorial(i), Fibonacci(i));
            std::cout << "---" << std::endl;
        }
        for (auto&& item : m) {
            std::cout << item.fa << " " << item.fi   << std::endl;
        }
    }

    return 0;
}