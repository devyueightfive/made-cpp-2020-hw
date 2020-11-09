#pragma once

#include <utility>

namespace task {

    struct ControlBlock {
        long shared_ptr_counter = 0;
        long weak_ptr_counter = 0;

        ControlBlock() = default;
    };

    template<class T>
    class WeakPtr;

    template<class T>
    class SharedPtr;

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
        friend WeakPtr<T>;
    private:
        pointer current_ptr = nullptr;
        ControlBlock *control_block = nullptr;
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
        SharedPtr(SharedPtr &&r) noexcept;

        /**
         *  Constructs a shared_ptr from WeakPtr.
         */
        SharedPtr(const WeakPtr<T> &w) noexcept;


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
         * Releases the ownership of the managed object, if any.
         * After the call, *this manages no object. Equivalent to shared_ptr().swap(*this);
         */
        void reset() noexcept;

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

//        friend WeakPtr<T>::WeakPtr(const SharedPtr &s) noexcept;

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

        friend SharedPtr<T>;
    private:
        pointer current_ptr = nullptr;
        ControlBlock *control_block = nullptr;
    public:
        // constructors

        /**
         * Empty pointer.
         */
        WeakPtr() noexcept;

        /**
         * Constructs new weak_ptr which shares an object managed by r.
         * If r manages no object, *this manages no object too.
         */
        WeakPtr(const SharedPtr<T> &s) noexcept;

        /**
         * Constructs new weak_ptr which shares an object managed by r.
         * If r manages no object, *this manages no object too.
         */
        WeakPtr(const WeakPtr &r) noexcept;

        /**
         * Move constructors. Moves a weak_ptr instance from r into *this.
         * After this, r is empty and r.use_count()==0
         */
        WeakPtr(WeakPtr &&w) noexcept;

        /**
         * Destroys a weak_ptr
         */
        ~WeakPtr();

        /**
         * Replaces the managed object with the one managed by r.
         * The object is shared with r. If r manages no object, *this manages no object too.
         */
        WeakPtr &operator=(const WeakPtr &r) noexcept;

        WeakPtr &operator=(const SharedPtr<T> &r) noexcept;

        /**
         * Move operator=.
         */
        WeakPtr &operator=(WeakPtr &&r) noexcept;

        //observers

        /**
         * Returns the number of shared_ptr instances that share ownership of the managed object,
         * or ​0​ if the managed object has already been deleted, i.e. *this is empty.
         */
        long use_count() const noexcept;

        /**
         * Equivalent to use_count() == 0. The destructor for the managed object may not yet have been called,
         * but this object's destruction is imminent (or may have already happened).
         */
        bool expired() const noexcept;

        /**
         * Creates a new std::shared_ptr that shares ownership of the managed object.
         * If there is no managed object, i.e. *this is empty, then the returned shared_ptr also is empty.
         */
        SharedPtr<T> lock() const noexcept;

        // modifiers

        /**
         * Releases the reference to the managed object. After the call *this manages no object.
         */
        void reset() noexcept;

        /**
         * Exchanges the contents of *this and r.
         */
        void swap(WeakPtr &r) noexcept;


//        friend SharedPtr<T>::SharedPtr(const WeakPtr<T> &w) noexcept;

    };


}  // namespace task


#include "smart_pointers.tpp"
