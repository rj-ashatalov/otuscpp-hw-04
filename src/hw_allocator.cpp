#include <cstdlib>
#include <new>
#include <iostream>
#include <limits>
#include <vector>

struct NoCopyable
{
    NoCopyable() = default;
    NoCopyable(const NoCopyable& other) = delete;
    virtual ~NoCopyable()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    };
};

struct Hard : public NoCopyable
{
    int fa;
    int fi;
    Hard(int fa, int fi)
            : fa(fa)
            , fi(fi)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
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
    using value_type = T;

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
bool operator==(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return std::is_same<T,U>::value; }
template <class T, class U>
bool operator!=(const SimpleAllocator<T>&, const SimpleAllocator<U>&) { return !std::is_same<T,U>::value; }


template <class T, size_t N>
struct HwAllocator
{
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = HwAllocator<U, N>;
    };

    HwAllocator() = default;

    template <class U>
    constexpr HwAllocator(const HwAllocator<U, N>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        if (n > _capacityRemain)
        {
            auto reserveCapacity =  N;
            if (n > reserveCapacity)
            {
                reserveCapacity = n;
            }

            _lastPointer = reserve(reserveCapacity);
            _mempool.emplace_back(_lastPointer);
            _capacityRemain = reserveCapacity;
        }

        _size += n;
        _capacityRemain -= n;
        auto currentPointer = _lastPointer;
        _lastPointer += n;
        return currentPointer;
    }

    void deallocate(T*, std::size_t n) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if (_mempool.size() <= 0)
        {
            return;
        }
        _size -= n;
        if(_size <= 0)
        {
            std::cout << "Free all memory" << std::endl;
            for (auto &&chunk : _mempool)
            {
                std::free(chunk);
            }
            _mempool.clear();
        }
    }

//    template<typename U, typename ...Args>
//    void construct(U *p, Args &&...args) const {
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//        new(p) U(std::forward<Args>(args)...);
//    };
//
//    void destroy(T *p) const {
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//        p->~T();
//    }

    private:
        size_t _size = 0u;
        size_t _capacityRemain = 0u;
        std::vector<T*> _mempool;
        T * _lastPointer = nullptr;

        T* reserve(std::size_t n)
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
};
template <class T, size_t N, class U, size_t M>
bool operator==(const HwAllocator<T, N>&, const HwAllocator<U, M>&) { return (N == M) && std::is_same<T,U>::value; }
template <class T, size_t N, class U, size_t M>
bool operator!=(const HwAllocator<T, N>&, const HwAllocator<U, M>&) { return (N != M) || !std::is_same<T,U>::value; }
