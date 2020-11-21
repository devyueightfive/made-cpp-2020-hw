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
        if (this == &right) {
            return *this;
        }
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
    SharedPtr<T>::SharedPtr() noexcept = default;

    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr::pointer ptr) : current_ptr(ptr) {
        control_block = new ControlBlock();
        control_block->shared_ptr_counter += 1;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr &other) noexcept {
        if (other.control_block != nullptr) {
            current_ptr = other.current_ptr;
            control_block = other.control_block;
            control_block->shared_ptr_counter += 1; // increase as new copy
        }
    }

    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr &&r) noexcept {
        this->swap(r);
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const WeakPtr<T> &w) noexcept {
        if (w.current_ptr != nullptr) {
            current_ptr = w.current_ptr;
            control_block = w.control_block;
            control_block->shared_ptr_counter += 1;
        }
    }

    template<class T>
    typename SharedPtr<T>::pointer SharedPtr<T>::get() const noexcept {
        return current_ptr;
    }

    template<class T>
    void SharedPtr<T>::reset(SharedPtr::pointer ptr) {
        // manage old state of *this
        if (current_ptr != nullptr) {
            --control_block->shared_ptr_counter;
            if (control_block->shared_ptr_counter == 0) {
                delete current_ptr;
            }
            if (control_block->shared_ptr_counter + control_block->weak_ptr_counter == 0) {
                delete control_block;
            }
        }
        // SharedPtr<T>(ptr).swap(*this);
        current_ptr = ptr;
        control_block = new ControlBlock();
        control_block->shared_ptr_counter += 1;
    }

    template<class T>
    void SharedPtr<T>::reset() noexcept {
        // manage old state of *this
        if (current_ptr != nullptr) {
            --control_block->shared_ptr_counter;
            if (control_block->shared_ptr_counter == 0) {
                delete current_ptr;
            }
            if (control_block->shared_ptr_counter + control_block->weak_ptr_counter == 0) {
                delete control_block;
            }
        }
        // SharedPtr<T>().swap(*this);
        current_ptr = nullptr;
        control_block = nullptr;
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
            return control_block->shared_ptr_counter;
        }
    }

    template<class T>
    SharedPtr<T>::~SharedPtr() {
        if (control_block != nullptr) {
            --control_block->shared_ptr_counter;
            if (control_block->shared_ptr_counter == 0) {
                if (current_ptr != nullptr) {
                    delete current_ptr;
                }
            }
            if (control_block->shared_ptr_counter + control_block->weak_ptr_counter == 0) {
                delete control_block;
            }
        }
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &r) noexcept {
        if (this == &r) {
            return *this;
        }
        SharedPtr<T>(r).swap(*this);
        return *this;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr &&r) noexcept {
        if (this == &r) {
            return *this;
        }
        SharedPtr<T>(std::move(r)).swap(*this);
        return *this;
    }


    template<class T>
    WeakPtr<T>::WeakPtr() noexcept = default;

    template<class T>
    WeakPtr<T>::WeakPtr(const SharedPtr<T> &s) noexcept:
            current_ptr(s.get()) {
        if (current_ptr != nullptr) {
            control_block = s.control_block;
            control_block->weak_ptr_counter += 1;
        }
    }

    template<class T>
    WeakPtr<T>::WeakPtr(const WeakPtr &r) noexcept:
            current_ptr(r.current_ptr) {
        if (current_ptr != nullptr) {
            control_block = r.control_block;
            control_block->weak_ptr_counter += 1;
        }
    }

    template<class T>
    WeakPtr<T>::WeakPtr(WeakPtr &&w) noexcept {
        this->swap(w);
    }

    template<class T>
    WeakPtr<T>::~WeakPtr() {
        if (control_block != nullptr) {
            --control_block->weak_ptr_counter;
            if (control_block->weak_ptr_counter + control_block->shared_ptr_counter == 0) {
                delete control_block;
            }
        }
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr &r) noexcept {
        if (this == &r) {
            return *this;
        }
        WeakPtr<T>(r).swap(*this);
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr<T> &r) noexcept {
        WeakPtr<T>(r).swap(*this);
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr &&r) noexcept {
        if (this == &r) {
            return *this;
        }
        WeakPtr<T>(std::move(r)).swap(*this);
        return *this;
    }

    template<class T>
    long WeakPtr<T>::use_count() const noexcept {
        if (control_block == nullptr) {
            return 0L;
        }
        return control_block->shared_ptr_counter;
    }

    template<class T>
    bool WeakPtr<T>::expired() const noexcept {
        return use_count() == 0;
    }

    template<class T>
    SharedPtr<T> WeakPtr<T>::lock() const noexcept {
        return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
    }

    template<class T>
    void WeakPtr<T>::reset() noexcept {
        current_ptr = nullptr;
        if (control_block != nullptr) {
            --control_block->weak_ptr_counter;
        }
    }

    template<class T>
    void WeakPtr<T>::swap(WeakPtr &r) noexcept {
        std::swap(current_ptr, r.current_ptr);
        std::swap(control_block, r.control_block);
    }
} //task namespace