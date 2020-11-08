#include "smart_pointers.h"

namespace task {

    // Definitions

    template<class T>
    UniquePtr<T>::UniquePtr(pointer p) noexcept: current_ptr(p) {}

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
    UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&r) noexcept {
        reset(r.release());
        return *this;
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::release() noexcept {
        pointer old_ptr = get();
        this->current_ptr = nullptr;
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
} //task namespace