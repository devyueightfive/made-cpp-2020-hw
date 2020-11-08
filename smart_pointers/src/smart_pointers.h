#pragma once

#include <utility>

namespace task {

    /**
     * Class unique_ptr is a smart pointer that owns and manages another object through a pointer
     * and disposes of that object when the unique_ptr goes out of scope.
     * */

    template<class T>
    class UniquePtr {

    public:
        /// Member types
        using pointer = T *; // std::remove_reference<T>::type::pointer
        using element_type = T;
    private:
        pointer current_ptr = nullptr;

    public:

        // constructors

        /** Constructor from pointer:
        * initializing the stored pointer with p
        */
        UniquePtr(pointer p) noexcept;

        // disable copy constructor
        UniquePtr(const UniquePtr &) = delete;

        /** Move constructor:
        * Constructs a unique_ptr by transferring ownership from u to *this and stores the null pointer in u.
        */
        UniquePtr(UniquePtr &&u) noexcept;

        // destructor
        ~UniquePtr();

        // assignment
        /**
        * Disable copy operator.
         */
        UniquePtr &operator=(const UniquePtr &) = delete;

        /**
          * Move assignment operator. Transfers ownership from r to *this as if by calling reset(r.release()).
          */
        UniquePtr &operator=(UniquePtr &&r) noexcept;

        // modifiers

        /**
         * Returns a pointer to the managed object and releases the ownership.
         */
        pointer release() noexcept;

        /**
         * Replaces the managed object.
         */
        void reset(pointer ptr) noexcept;

        /**
         * Swaps the managed objects of *this and another unique_ptr object other.
         */
        void swap(UniquePtr &other) noexcept;

        // observers

        /**
         * Returns a pointer to the managed object.
         */
        pointer get() const noexcept;

        /**
         * Provide access to the object owned by *this.
         * The behavior is undefined if get() == nullptr.
         */
        element_type &operator*() const;

        /**
         * Provide access to the object owned by *this.
         * The behavior is undefined if get() == nullptr.
         */
        pointer operator->() const noexcept;
    };

    template<class T>
    class SharedPtr {
        // Your code goes here...
    };

    template<class T>
    class WeakPtr {
        // Your code goes here...
    };


}  // namespace task


#include "smart_pointers.tpp"
