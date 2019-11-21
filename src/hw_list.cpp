#include <memory>
#include <iostream>
#include <vector>

template<class T,  class Alloc = std::allocator<T> >
class HwList
{
    public:
        using value_type = T;
        using allocator_type  = Alloc;
        using allocator_traits = std::allocator_traits<allocator_type>;
        using pointer = typename allocator_traits::pointer;
        using size_type  = typename allocator_traits::size_type;
        using reference = value_type&;
        using const_reference = value_type const&;
        using mem_catche = std::vector<pointer>;

        struct Iterator: public std::iterator< std::input_iterator_tag   // iterator_category
                                                ,value_type               // value_type
                                                ,size_type                // difference_type
                                                ,pointer               // pointer
                                                ,reference                       // reference
                                                >
        {
            const mem_catche& _refList;
            size_t _current = 0u;

            explicit Iterator(const mem_catche& refList, size_t index)
                : _refList(refList)
                , _current(index) {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            }

            Iterator& operator++()
            {
                ++_current;
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator retval = *this;
                ++(*this);
                return retval;
            }

            bool operator==(const Iterator& other) const
            {
                return _current == other._current;
            }

            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }

            reference operator*() const
            {
                if (_current < _refList.size())
                {
                    return *(_refList[_current]);
                }
                return *(_refList[std::max(static_cast<int>(_refList.size()) - 1, 0)] + 1);
            }
        };

    private:
        allocator_type _alloc;
        mem_catche _refList;

    public:
        HwList()
        {
            //
        };

        ~HwList()
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            for (auto ptr : _refList)
            {
                ptr->~T();
                _alloc.deallocate(ptr, 1);
            }
            _refList.clear();
        };

        template <class ...Args>
        void EmplaceBack(Args&&... args)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            const auto item = _alloc.allocate(1);
            new(item) T(std::forward<Args>(args)...);
            _refList.emplace_back(item);
        }

        constexpr Iterator begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_refList, 0);
        }

        constexpr Iterator end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_refList, _refList.size());
        }
};