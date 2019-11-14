#include <memory>
#include <iostream>

template<class T,  class Alloc = std::allocator<T> >
class HwList
{
    public:
        using value_type = T;
        using size_type  = std::size_t;
//        typedef value_type& reference;
//        typedef const value_type& const_reference;

        Alloc _alloc;
        size_type _size;
        T* _begin;

        HwList()
        {
            //
        };

        ~HwList()
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            for (size_t i = 0; i < _size; ++i)
            {
                _begin->~value_type();
            }
            _alloc.deallocate(_begin, sizeof(value_type)/*probably fake data*/);
        };

        template <class ...Args>
        void EmplaceBack(Args&&... args)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            const auto item = _alloc.allocate(1);
            new(item) T(std::forward<Args>(args)...);
            if (_begin == nullptr)
            {
                _begin = item;
            }
            ++_size;
        }

        constexpr const value_type* begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return _begin;
        }

        constexpr const value_type* end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return _begin + _size;
        }
};