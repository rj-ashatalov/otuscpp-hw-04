#include <vector>
#include <map>
#include "hw_allocator.cpp"


int main(int, char const *[])
{
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

    {
        std::cout << "\n-- Map with homework allocator -- " << std::endl;
        auto m2 = std::map<int, int, std::less<int>, HwAllocator<std::pair<const int, int>, 4>>{};
        for (size_t i = 0; i < 5; ++i) {
            m2.emplace(i, i);
            std::cout << "---" << std::endl;
        }
        for (auto[key, value]: m2) {
            std::cout << key << " " << value << std::endl;
        }
    }

    return 0;
}