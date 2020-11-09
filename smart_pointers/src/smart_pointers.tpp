#include "smart_pointers.h"

namespace task {

    // Definitions

    template<class T>
    UniquePtr<T>::UniquePtr(pointer ptr): current_ptr(ptr) {}

    template<class T>
    UniquePtr<T>::UniquePtr(UniquePtr &&u) noexcept {
        reset(u.release());
    }

    template<class T>
    UniquePtr<T>::~UniquePtr() {
        if (this->current_ptr == nullptr) {
            return;
        }
        delete this->current_ptr;
    }

    template<class T>
    UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&right) noexcept {
        reset(right.release());
        return *this;
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::release() noexcept {
        pointer old_ptr = current_ptr;
        current_ptr = nullptr;
        return old_ptr;
    }

    template<class T>
    void UniquePtr<T>::reset(pointer ptr) noexcept {
        pointer old_ptr = this->current_ptr;
        current_ptr = ptr;
        if (old_ptr != nullptr) {
            delete old_ptr;
        }
    }

    template<class T>
    void UniquePtr<T>::swap(UniquePtr &other) noexcept {
        std::swap(this->current_ptr, other.current_ptr);
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::get() const noexcept {
        return this->current_ptr;
    }

    template<class T>
    typename UniquePtr<T>::element_type &UniquePtr<T>::operator*() const {
        return *get();
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::operator->() const noexcept {
        return get();
    }


    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr::pointer ptr) : current_ptr(ptr), control_block(new long(1)) {
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr &other) noexcept {
        if (other.current_ptr != nullptr) {
            if (this != &other) {
                current_ptr = other.current_ptr;
                control_block = other.control_block;
            }
            *control_block += 1;
        }
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr &&r) noexcept {
        current_ptr = r.current_ptr;
        control_block = r.control_block;
        r.current_ptr = nullptr;
        r.control_block = nullptr;
    }

    template<class T>
    typename SharedPtr<T>::pointer SharedPtr<T>::get() const noexcept {
        return current_ptr;
    }

    template<class T>
    void SharedPtr<T>::reset(SharedPtr::pointer ptr) {
        // manage old state of *this
        if (current_ptr != nullptr) {
            if (*control_block == 1) {
                delete current_ptr;
                delete control_block;
            } else {
                *control_block -= 1;
            }
        }
        current_ptr = ptr;
        control_block = new long(1);
    }

    template<class T>
    void SharedPtr<T>::swap(SharedPtr &r) noexcept {
        std::swap(current_ptr, r.current_ptr);
        std::swap(control_block, r.control_block);
    }

    template<class T>
    typename SharedPtr<T>::element_type &SharedPtr<T>::operator*() const noexcept {
        return *get();
    }

    template<class T>
    typename SharedPtr<T>::pointer SharedPtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    long SharedPtr<T>::use_count() const noexcept {
        if (current_ptr == nullptr) {
            return 0;
        } else {
            return *control_block;
        }
    }

    template<class T>
    SharedPtr<T>::~SharedPtr() {
        if (current_ptr != nullptr) {
            if (*control_block == 1) {
                delete current_ptr;
                delete control_block;
            } else {
                *control_block -= 1;
            }
        }
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &r) noexcept {
        SharedPtr<T>(r).swap(*this);
        return *this;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr &&r) noexcept {
        SharedPtr<T>(std::move(r)).swap(*this);
        return *this;
    }

    template<class T>
    SharedPtr<T>::SharedPtr() noexcept = default;




} //task namespace