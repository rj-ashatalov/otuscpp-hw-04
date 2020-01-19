#include <memory>
#include <iostream>

template<class T, class Alloc = std::allocator<T> >
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

    private:
        struct Node
        {
            pointer value = nullptr;
            Node* next = nullptr;

            ~Node()
            {
                if(next)
                {
                    delete next;
                }
            }

            bool operator==(const Node& other)
            {
                return value == other.value
                       && next == other.next;
            }
        };
        Node* _head = nullptr;
        Node* _tail = nullptr;
        Node* _end = new Node{};

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
                return *(_current->value);
            }
        };

        HwList() = default;

        constexpr HwList(const HwList<T, Alloc>& other) noexcept
                : _alloc(other._alloc)
                , _head(other._head)
                , _tail(other._tail)
                , _end(other.end)
        {
        };

        constexpr HwList(HwList<T, Alloc>&& other) noexcept
                : _alloc(std::move(other._alloc))
                , _head(std::move(other._head))
                , _tail(std::move(other._tail))
                , _end(std::move(other.end))
        {
        };

        ~HwList()
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            for (auto&& ptr : *this)
            {
                _alloc.destroy(&ptr);
                _alloc.deallocate(&ptr, 1);
            }

            delete _head;
        }

        template<class U, class AllocU>
        constexpr HwList(const HwList<U, AllocU>& other) = delete;

        template<class U, class AllocU>
        constexpr HwList(HwList<T, Alloc>&& other) = delete;

        template<class ...Args>
        void EmplaceBack(Args&& ... args)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            auto item = _alloc.allocate(1);
            _alloc.construct(item, std::forward<Args>(args)...);

            auto node = new Node();
            node->value = std::move(item);
            node->next = _end;
            if(_tail)
            {
                _tail->next = node;
            }
            _tail = node;
            if (!_head)
            {
                _head = _tail;
            }
        }

        constexpr Iterator begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_head);
        }

        constexpr Iterator end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_end);
        }
};