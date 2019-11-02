#include <cstddef>

struct NoCopyable
{
    NoCopyable() = default;
    NoCopyable(const NoCopyable& other) = delete;
    NoCopyable(NoCopyable&& other) = delete;
    virtual ~NoCopyable() = default;
};

struct hard : public NoCopyable
{
    int fa;
    int fi;
    hard(int fa,int fi) : fa(fa), fi(fi)
    {
        //emply
    };
};

template <class T>
struct HwAllocator
{
    T* allocate(std::size_t n);
    {
        aut op = std::malloc(n *sizeof(T));
        if (!p)
        {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T *> (p);
    };

    void deallocate(T* p, std::size_t n);
    {
        std::free(p);
    };
};