#include <memory>
#include <iostream>

template<class T, class Alloc = std::allocator<T> >
class HwList
{
        template<class U>
        struct ForwardList
        {
            struct Node
            {
                U value = nullptr;
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

            Node* head = nullptr;
            Node* tail = nullptr;
            Node* end = new Node{};

            ForwardList() = default;

            constexpr ForwardList(const ForwardList<U>& other) noexcept
                    : head(other.head)
                    , tail(other.tail)
                    , end(other.end)
            {
            };

            constexpr ForwardList(ForwardList<U>&& other) noexcept
                    : head(std::move(other.head))
                    , tail(std::move(other.tail))
                    , end(std::move(other.end))
            {
            };

            ~ForwardList()
            {
                delete head;
            }

            void EmplaceBack(U&& item)
            {
                auto node = new typename mem_catche::Node();
                node->value = std::forward<U>(item);
                node->next = ForwardList<U>::end;
                if(tail)
                {
                    tail->next = node;
                }
                tail = node;
                if (!head)
                {
                    head = tail;
                }
            }
        };

    public:
        using value_type = T;
        using allocator_type  = Alloc;
        using allocator_traits = std::allocator_traits<allocator_type>;
        using pointer = typename allocator_traits::pointer;
        using size_type  = typename allocator_traits::size_type;
        using reference = value_type&;
        using const_reference = value_type const&;
        using mem_catche = ForwardList<pointer>;

        struct Iterator: public std::iterator<std::input_iterator_tag   // iterator_category
                , value_type               // value_type
                , size_type                // difference_type
                , pointer               // pointer
                , reference                       // reference
        >
        {
            typename mem_catche::Node* _current = nullptr;

            explicit Iterator(typename mem_catche::Node* node)
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

    private:
        allocator_type _alloc;
        mem_catche _refList;

    public:
        HwList()
        {
            //
        };

        constexpr HwList(const HwList<T, Alloc>& other) noexcept
                : _alloc(other._alloc)
                , _refList(other._refList)
        {
        };

        constexpr HwList(HwList<T, Alloc>&& other) noexcept
                : _alloc(std::move(other._alloc))
                , _refList(std::move(other._refList))
        {
        };

        template<class U, class AllocU>
        constexpr HwList(const HwList<U, AllocU>& other) = delete;

        template<class U, class AllocU>
        constexpr HwList(HwList<T, Alloc>&& other) = delete;

        ~HwList()
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            for (auto&& ptr : *this)
            {
                _alloc.destroy(&ptr);
                _alloc.deallocate(&ptr, 1);
            }
        };

        template<class ...Args>
        void EmplaceBack(Args&& ... args)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            auto item = _alloc.allocate(1);
            _alloc.construct(item, std::forward<Args>(args)...);
            _refList.EmplaceBack(std::move(item));
       }

        constexpr Iterator begin() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_refList.head);
        }

        constexpr Iterator end() noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return Iterator(_refList.end);
        }
};