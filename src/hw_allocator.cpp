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

struct HwAllocator
{

};