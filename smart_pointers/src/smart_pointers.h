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
        explicit UniquePtr(pointer ptr);

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

    /**
     * SharedPtr is a smart pointer that retains shared ownership of an object through a pointer.
     * Several SharedPtr objects may own the same object.
     */

    template<class T>
    class SharedPtr {
    public:
        using pointer = T *;
        using element_type = T;
        using weak_type = WeakPtr<T>;
    private:
        pointer current_ptr = nullptr;
        long *control_block = nullptr;
    public:

        // constructors
        /**
         * Constructs a shared_ptr with no managed object, i.e. empty shared_ptr
         */
        SharedPtr() noexcept;

        /**
         * Constructs a shared_ptr with ptr as the pointer to the managed object.
         */
        explicit SharedPtr(pointer ptr);

        /**
         * Constructs a shared_ptr which shares ownership of the object managed by other.
         * If other manages no object, *this manages no object too.
         */
        SharedPtr(const SharedPtr &other) noexcept;

        /**
         *  Move-constructs a shared_ptr from r. After the construction,
         *  *this contains a copy of the previous state of r, r is empty and its stored pointer is null.
         */
        SharedPtr(const SharedPtr &&r) noexcept;

        /**
         *  Constructs a shared_ptr which shares ownership of the object managed by r.
         */
        explicit SharedPtr(const weak_type &r) noexcept;


        /**
         * If *this owns an object and it is the last shared_ptr owning it, the object is destroyed.
         */
        ~SharedPtr();

        // operator=

        /**
         * Replaces the managed object with the one managed by r.
         * If *this already owns an object and it is the last shared_ptr owning it, and r is not the same as *this,
         * the object is destroyed
         */
        SharedPtr &operator=(const SharedPtr &r) noexcept;

        SharedPtr &operator=(SharedPtr &&r) noexcept;


        // modifiers

        /**
         * Replaces the managed object with an object pointed to by ptr.
         * If *this already owns an object and it is the last shared_ptr owning it, the object is destroyed.
         * If the object pointed to by ptr is already owned, the function generally results in undefined behavior.
         * @ptr	-	pointer to an object to acquire ownership of.
         */
        void reset(pointer ptr);

        /**
         * Exchanges the contents of *this and r.
         * @r - smart pointer to exchange the contents with.
         */
        void swap(SharedPtr &r) noexcept;

        //observers

        /**
         * Returns the stored pointer.
         */
        pointer get() const noexcept;

        /**
         * Returns the result of dereferencing the stored pointer, i.e., *get().
         * The behavior is undefined if the stored pointer is null.
         */
        element_type &operator*() const noexcept;

        /**
         * Returns the stored pointer, i.e., get().
         */
        pointer operator->() const noexcept;

        /**
         * Returns the number of different shared_ptr instances (this included) managing the current object.
         * If there is no managed object, ​0​ is returned.
         */
        long use_count() const noexcept;

    };


    /**
     * WeakPtr is a smart pointer that holds a non-owning ("weak") reference
     * to an object that is managed by SharedPtr. It must be converted to SharedPtr
     * in order to access the referenced object.
     */

    template<class T>
    class WeakPtr {
    public:
        using pointer = T *;
        using element_type = T;
    private:
        pointer current_ptr = nullptr;
    public:
        // constructors

        WeakPtr() noexcept;

        explicit WeakPtr(const SharedPtr<T> &s) noexcept;

        WeakPtr(const WeakPtr<T> &r) noexcept;

        WeakPtr(const WeakPtr<T> &&r) noexcept;

        ~WeakPtr();

        WeakPtr &operator=(WeakPtr &r);

        WeakPtr &operator=(WeakPtr &&r);

        //observers

        long use_count();

        bool expired() const noexcept;

        SharedPtr<T> lock() const noexcept;

        // modifiers

        void reset() noexcept;

        void swap();

    };

    template<class T>
    WeakPtr<T>::WeakPtr(SharedPtr &s) {

    }

}  // namespace task


#include "smart_pointers.tpp"
