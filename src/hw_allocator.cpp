#include <cstdlib>
#include <new>
#include <iostream>
#include <limits>

struct NoCopyable
{
    NoCopyable() = default;
    NoCopyable(const NoCopyable& other) = delete;
    virtual ~NoCopyable() = default;
};

struct Hard : public NoCopyable
{
    int fa;
    int fi;
    Hard(int fa,int fi)
            : fa(fa)
            , fi(fi)
    {
        //emply
    };

    Hard(Hard&& other) noexcept
            : fa(std::move(other.fa))
            , fi(std::move(other.fi))
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    };
};

template <class T>
struct SimpleAllocator
{
    typedef T value_type;

    SimpleAllocator() = default;

    template <class U>
    constexpr SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        if(auto p = static_cast<T*>(std::malloc(n*sizeof(T))))
        {
            return p;
        }
        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::free(p);
    }
};
template <class T, class U>
bool operator==(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return std::is_same_v<T,U>; }
template <class T, class U>
bool operator!=(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return !std::is_same_v<T,U>; }


template <class T, size_t N>
struct HwAllocator
{
    typedef T value_type;

    HwAllocator() = default;

    template <class U, size_t M>
    constexpr HwAllocator(const HwAllocator<U, M>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        if(auto p = static_cast<T*>(std::malloc(n*sizeof(T))))
        {
            return p;
        }
        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::free(p);
    }
};
template <class T, size_t N, class U, size_t M>
bool operator==(const HwAllocator<T, N>&, const HwAllocator<U, M>&) { return (N == M) && std::is_same_v<T,U>; }
template <class T, size_t N, class U, size_t M>
bool operator!=(const HwAllocator<T, N>&, const HwAllocator<U, M>&) { return (N != M) || !std::is_same_v<T,U>; }

/*
template <class T, size_t N>
struct HwAllocator
{
    using value_type = T;

    T* allocate(std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if (n > _allocRemain)
        {
            auto p = std::malloc(N * sizeof(T));
            if (!p)
            {
                throw std::bad_alloc();
            }
            _allocRemain = N;
            //todo перераспределить элементы
            return reinterpret_cast<T *> (p);
        }
        else
        {
            _allocRemain -= n;
            auto currentPointer = _lastPointer;
            _lastPointer += n;
            return currentPointer;
        }
    };

    void deallocate(T* p, std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::free(p);
    };

    private:
        size_t _allocRemain = 0u;
        T * _lastPointer = nullptr;
};*/
