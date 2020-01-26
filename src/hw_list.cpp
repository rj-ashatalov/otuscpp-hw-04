#include <memory>
#include <iostream>
#include "no_copyable.h"

template<class T, class Alloc = std::allocator<T> >
class HwList
{
        struct Node;
    public:
        using value_type = T;
        using allocator_type  = typename Alloc::template rebind<Node>::other;
        using allocator_traits = std::allocator_traits<allocator_type>;
        using pointer = typename allocator_traits::pointer;
        using size_type  = typename allocator_traits::size_type;
        using reference = value_type&;
        using const_reference = value_type const&;

    private:
        struct Node: public NoCopyable
        {
            value_type value = nullptr;
            Node* next = nullptr;

            template<class ...Args>
            Node(Args&& ...args)
                    : value{std::forward<Args>(args)...}
            {
            }

            virtual ~Node() override
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            };

            bool operator==(const Node& other)
            {
                return value == other.value
                       && next == other.next;
            }
        };
        Node* _head = nullptr;
        allocator_type _alloc;

    public:
        struct Iterator: public std::iterator<std::input_iterator_tag   // iterator_category
                , value_type               // value_type
                , size_type                // difference_type
                , pointer               // pointer
                , reference                       // reference
        >
        {
            Node* _current = nullptr;

            explicit Iterator(Node* node)
                    : _current(node)
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            }

            Iterator& operator++()
            {
                if (_current)
                {
                    _current = _current->next;
                }
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
                return _current->value;
            }
        };

        HwList() = default;

        constexpr HwList(const HwList<T, Alloc>& other) noexcept
                : _alloc(other._alloc)
                , _head(other._head)
        {
        };

        constexpr HwList(HwList<T, Alloc>&& other) noexcept
                : _alloc(std::move(other._alloc))
                , _head(std::move(other._head))
        {
        };

        ~HwList()
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            auto node = _head;
            while (node != nullptr)
            {
                _alloc.destroy(node);
                _alloc.deallocate(node, 1);
                node = node->next;
            }
        }

        template<class U, class AllocU>
        constexpr HwList(const HwList<U, AllocU>& other) = delete;

        template<class U, class AllocU>
        constexpr HwList(HwList<T, Alloc>&& other) = delete;

        template<class ...Args>
        void EmplaceFront(Args&& ... args)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            auto node = _alloc.allocate(1);
            _alloc.construct(node, std::forward<Args>(args)...);

            if (_head)
            {
                node->next = _head;
            }
            _head = node;
        }

        constexpr Iterator begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_head);
        }

        constexpr Iterator end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(nullptr);
        }
};