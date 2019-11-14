#include <memory>
#include <iostream>

template<class T,  class Alloc = std::allocator<T> >
class HwList
{
    public:
        using value_type = T;
        using size_type  = std::size_t;
        using allocator_type  = Alloc;
        typedef value_type& reference;
        typedef value_type* pointer;
//        typedef const value_type& const_reference;

        allocator_type _alloc;
        size_type _size;
        T* _begin;

        struct Iterator: public std::iterator< std::input_iterator_tag   // iterator_category
                                                ,value_type               // value_type
                                                ,size_type                // difference_type
                                                ,pointer               // pointer
                                                ,reference                       // reference
                                                >
        {
            const HwList& _self;

            explicit Iterator(const HwList& self)
                : _self(self) {}

            Iterator& operator++()
            {
                num = TO >= FROM ? num + 1: num - 1;
                return *this;
            }
            Iterator operator++(int)
            {
                iterator retval = *this;
                ++(*this);
                return retval;
            }

            bool operator==(Iterator other) const {return num == other.num;}
            bool operator!=(Iterator other) const {return !(*this == other);}
            reference operator*() const {return num;}
        };

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

        constexpr const auto begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator{*this};
        }

        constexpr const auto end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return begin() + _size;
        }
};