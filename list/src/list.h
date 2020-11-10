#pragma once

#include <limits>
#include <iterator>
#include <memory>
#include <iostream>

#define assert(expr, msg) \
    if (!expr) std::cerr << msg << std::endl;


namespace task {


    template<class T, class Alloc = std::allocator<T>>
    class list {

    public:
        class iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator();

            iterator(const iterator &);

            iterator &operator=(const iterator &);

            iterator &operator++();

            iterator operator++(int);

            reference operator*() const;

            pointer operator->() const;

            iterator &operator--();

            iterator operator--(int);

            bool operator==(iterator other) const;

            bool operator!=(iterator other) const;

            // Your code goes here?..

        private:
            // Your code hoes here...
        };

        class const_iterator {
            // Your code goes here...
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        list();

        explicit list(const Alloc &alloc);

        list(size_t count, const T &value, const Alloc &alloc = Alloc());

        explicit list(size_t count, const Alloc &alloc = Alloc());

        ~list();

        list(const list &other);

        list(list &&other);

        list &operator=(const list &other);

        list &operator=(list &&other);

        Alloc get_allocator() const;


        T &front();

        const T &front() const;

        T &back();

        const T &back() const;


        iterator begin();

        iterator end();

        const_iterator cbegin() const;

        const_iterator cend() const;

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_reverse_iterator crbegin() const;

        const_reverse_iterator crend() const;


        bool empty() const;

        size_t size() const;

        size_t max_size() const;

        void clear();

        iterator insert(const_iterator pos, const T &value);

        iterator insert(const_iterator pos, T &&value);

        iterator insert(const_iterator pos, size_t count, const T &value);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);


        void push_back(const T &value);

        void push_back(T &&value);

        void pop_back();

        void push_front(const T &value);

        void push_front(T &&value);

        void pop_front();

        template<class... Args>
        iterator emplace(const_iterator pos, Args &&... args);

        template<class... Args>
        void emplace_back(Args &&... args);

        template<class... Args>
        void emplace_front(Args &&... args);

        void resize(size_t count);

        void swap(list &other);


        void merge(list &other);

        void splice(const_iterator pos, list &other);

        void remove(const T &value);

        void reverse();

        void unique();

        void sort();

        // Your code goes here?..

        /**
         * Member types
         */
    private:
        typedef std::allocator_traits<Alloc> _traits;
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename _traits::size_type size_type;
        typedef typename _traits::difference_type difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename _traits::pointer pointer;
        typedef typename _traits::const_pointer const_pointer;

        static_assert(
                std::is_same<typename Alloc::value_type, value_type>::value,
                "Allocator::value_type must be same type as value_type"
        );

        // typedef iterators


    private:
        // Your code goes here...

        class Node {
        public:
            Node *next = nullptr;
//            Node *prev = nullptr;
            pointer data = nullptr;
            allocator_type _allocator;

            Node() = default;

            Node(allocator_type &allocator, const T &value) :
                    _allocator(allocator) {
                data = _traits::allocate(_allocator, 1);
                _traits::construct(_allocator, data, value);
            }

            Node(allocator_type &allocator, T &&value) :
                    _allocator(allocator) {
                data = _traits::allocate(_allocator, 1);
                _traits::construct(_allocator, data, value);
            }

            ~Node() {
                _traits::destroy(_allocator, data);
                _traits::deallocate(_allocator, data, 1);
            }
        };

        Node *_head = nullptr;
        Node *_tail = nullptr;
        std::size_t _size = 0;
        allocator_type _allocator;


        void _erase(size_t index) {
            if (_size > 0) {
                if (index == 0) {
                    if (_size == 1) {
                        delete _head;
                        _head = _tail = nullptr;
                    } else {
                        Node *tmp = _head;
                        _head = _head->next;
                        delete tmp;
                    }
                } else {
                    Node *prev = _head;
                    Node *cur = prev->next;
                    std::size_t j(1);
                    while (j < index) {
                        prev = cur;
                        cur = prev->next;
                        j += 1;
                    }
                    prev->next = cur->next;
                    if (cur == _tail) {
                        _tail = prev;
                    }
                    delete cur;
                }
            }
            _size -= 1;
        }


        template<class... Args>
        void _emplace_back(Args &&... args) {

        }
    };

    /**
     * Default constructor. Constructs an empty container with a default-constructed allocator.
     */
    template<class T, class Alloc>
    list<T, Alloc>::list() :
            _allocator() {};

    /**
     * Constructs an empty container with the given allocator alloc.
     */
    template<class T, class Alloc>
    list<T, Alloc>::list(const Alloc &alloc) :
            _allocator(alloc) {}

    /**
     * Appends the given element value to the end of the container.
     * The new element is initialized as a copy of value.
     */
    template<class T, class Alloc>
    void list<T, Alloc>::push_back(const T &value) {
        Node *new_node = new Node(_allocator, value);
        if (_size == 0) {
            _head = _tail = new_node;
        } else {
            _tail->next = new_node;
            _tail = new_node;
        }
        _size += 1;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_back(T &&value) {
        Node *new_node = new Node(_allocator, value);
        if (_size == 0) {
            _head = _tail = new_node;
        } else {
            _tail->next = new_node;
            _tail = new_node;
        }
        _size += 1;
    }

    /**
     * Removes the last element of the container.
     * Calling pop_back on an empty container results in undefined behavior.
     */
    template<class T, class Alloc>
    void list<T, Alloc>::pop_back() {
        _erase(_size - 1);
    }

    /**
     * Removes the first element of the container.
     */
    template<class T, class Alloc>
    void list<T, Alloc>::pop_front() {
        _erase(_size - 1);
    }

    /**
     * Returns the number of elements in the container, i.e. std::distance(begin(), end()).
     */
    template<class T, class Alloc>
    size_t list<T, Alloc>::size() const {
        return _size;
    }

    /**
     * Constructs the container with count copies of elements with value value.
     */
    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc):
            _allocator(alloc) {
        while (size() != count)
            push_back(value);
    }


    /**
     * Constructs the container with count default-inserted instances of T. No copies are made.
     */
    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const Alloc &alloc) {

    }

    /**
     * Returns a reference to the first element in the container.
     * Calling front on an empty container is undefined.
     * For a container c, the expression c.front() is equivalent to *c.begin().
     */
    template<class T, class Alloc>
    T &list<T, Alloc>::front() {
        return *_head->data;
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::front() const {
        return *_head->data;
    }

    /**
     * Returns a reference to the last element in the container.
     * Calling back on an empty container causes undefined behavior.
     * For a non-empty container c, the expression c.back() is equivalent to *std::prev(c.end())
     */
    template<class T, class Alloc>
    T &list<T, Alloc>::back() {
        return *_tail->data;
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::back() const {
        return *_tail->data;
    }

    /**
     * Checks if the container has no elements, i.e. whether begin() == end().
     */
    template<class T, class Alloc>
    bool list<T, Alloc>::empty() const {
        return _size == 0;
    }

    /**
     * Returns the allocator associated with the container.
     */
    template<class T, class Alloc>
    Alloc list<T, Alloc>::get_allocator() const {
        return _allocator;
    }

    /**
     * Returns the maximum number of elements the container is able to hold due to system
     * or library implementation limitations, i.e. std::distance(begin(), end()) for the largest container.
     *
     * This value typically reflects the theoretical limit on the size of the container,
     * at most std::numeric_limits<difference_type>::max()
     */
    template<class T, class Alloc>
    size_t list<T, Alloc>::max_size() const {
        return _traits::max_size(_allocator);
    }

    template<class T, class Alloc>
    template<class... Args>
    void list<T, Alloc>::emplace_back(Args &&... args) {

    }














// Your template function definitions may go here...

}  // namespace task
