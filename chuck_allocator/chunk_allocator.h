#ifndef HW_5_ALLOCATOR_MAIN_H
#define HW_5_ALLOCATOR_MAIN_H

#include <cstdlib>
#include <memory>
#include <iostream>
#include <list>
#include <stdexcept>
//#include <algorithm>


namespace task {

    class Chunk {
    private:
        uint8_t *p; // pointer to block with CHUNK_SIZE bytes
        size_t size_of_data; // size of block part of spent memory
    public:
        const static size_t CHUNK_SIZE = 100; //bytes
        Chunk *prev;

        Chunk();

        Chunk(Chunk *prev, size_t size);

        void allocate(size_t size);

        Chunk(const Chunk &other);

        bool is_empty();

        size_t get_free_size() const;

        uint8_t *add_block(std::size_t n);

        ~Chunk();
    };

    template<typename T>
    class MyAllocator {
        static uint copies;

    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using difference_type = int; // signed
        using const_reference = const T &;
        using size_type = std::size_t;
        template<class U>
        struct rebind {
            typedef MyAllocator<U> other;
        };

//    std::list<pointer> elements;
    public:
        MyAllocator();

        // Copy constructor
        MyAllocator(const MyAllocator &other);

        MyAllocator<T> &operator=(MyAllocator<T> const &other);

        T *allocate(size_t n);

        size_type max_size();

        void deallocate(T *p, size_type n) noexcept;

        template<typename ... Args>
        void construct(T *p, Args &&... args);

        void destroy(T *p);

        ~MyAllocator();


    private:
        Chunk *chunks = nullptr; // chunks

        void report(T *p, size_type n, bool alloc = true) {
            std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                      << " bytes at " << std::hex << std::showbase
                      << reinterpret_cast<void *>(p) << std::dec << std::endl;
        }

        void self_report() {
            std::cout << "i'm : " << std::hex << std::showbase
                      << reinterpret_cast<void *>(this) << std::dec
                      << " copy is " << this->copies << std::endl;
        }
    };


    Chunk::Chunk() : p(nullptr), size_of_data(0), prev(nullptr) {
//    std::cout << "Chunk()" << std::endl;
    };

    Chunk::Chunk(Chunk *prev, size_t size = Chunk::CHUNK_SIZE) : size_of_data(0), prev(prev) {
        p = new uint8_t[size];
//    std::cout << "Chunk() at " << std::hex << std::showbase
//              << reinterpret_cast<void *>(p) << std::dec << std::endl;
    }

    void Chunk::allocate(size_t size = CHUNK_SIZE) {
        p = new uint8_t[size];
//    std::cout << "Chunk() at " << std::hex << std::showbase
//              << reinterpret_cast<void *>(p) << std::dec << std::endl;
    }


    Chunk::Chunk(const Chunk &other) {
//    std::cout << "Copy Chunk() at " << std::hex << std::showbase
//              << reinterpret_cast<void *>(p) << std::dec << std::endl;
        if (this != &other) {
            delete[] static_cast<uint8_t *>(p);
            prev = other.prev;
            p = other.p;
            size_of_data = other.size_of_data;
//            this->copies += 1;
        }
    }

    bool Chunk::is_empty() {
        if (p == nullptr) {
//        std::cout << "Chunk is empty." << std::endl;
        }
        return (p == nullptr);
    }

    size_t Chunk::get_free_size() const {
        return Chunk::CHUNK_SIZE - size_of_data;
    }

    uint8_t *Chunk::add_block(std::size_t n) {
        if (n > get_free_size()) {
//        std::cout << "Chunk: requested more bytes than defined in CHUNK_SIZE while your n is (" << n
//                  << ")" << std::endl;
            throw std::bad_alloc();
        }
        auto return_pointer = p + size_of_data;
        size_of_data += n;
//    std::cout << "New block at " << std::hex << std::showbase
//              << reinterpret_cast<void *>(return_pointer)
//              << " ends in " << reinterpret_cast<void *>(p + size_of_data) << std::dec
//              << " with size " << n << " bytes" << std::endl;
        return return_pointer;
    }


    Chunk::~Chunk() {
        delete[] static_cast<uint8_t *>(p);
//    std::cout << "~Chunk()" << std::endl;
    }

    template<typename T>
    uint MyAllocator<T>::copies = 1;

    template<typename T>
    MyAllocator<T>::MyAllocator(): chunks(new Chunk()) {
//    self_report();
//    std::cout << "MyAllocator()\n";
    };

// Copy constructor
    template<typename T>
    MyAllocator<T>::MyAllocator(const MyAllocator &other) {
//    self_report();
//    std::cout << "copy MyAllocator()\n";
        if (this != &other) {
            MyAllocator::copies += 1;
            delete this->chunks; // Destruct previous version
            this->chunks = other.chunks;
        }
    };

    template<typename T>
    MyAllocator<T> &MyAllocator<T>::operator=(const MyAllocator<T> &other) {
//    self_report();
//    std::cout << "copy MyAllocator()\n";
        if (this != &other) {
            MyAllocator::copies += 1;
            delete this->chunks; // Destruct previous version of chunks
            this->chunks = other.chunks;
        }
        return *this;
    }

    template<typename T>
    T *MyAllocator<T>::allocate(size_t n) {
//    self_report();
//    std::cout << "Allocating " << n << " items" << std::endl;

        // Request capacity more than Chunk::CHUNK_SIZE
        if (n > max_size()) {
            std::cout << "Request amount of memory more than " << Chunk::CHUNK_SIZE << " bytes." << std::endl;
            throw std::bad_alloc();
        }

        // If list of chunks is empty add chunk.
        if (chunks->is_empty()) {
            chunks->allocate();
        }

        // Pointer to return. Points to n * sizeof(T) free block.
        T *p = nullptr;
        bool found = false;

//        std::cout << "Searching free space ..." << std::endl;
        // Add n elements. Go through list of chunks to find free place
        // for requested amount of memory.
        Chunk *it = chunks;
        while (true) {
            if (it->get_free_size() >= n * sizeof(T)) {
                p = reinterpret_cast<T *>(it->add_block(n * sizeof(T)));
                found = true;
//                std::cout << "Found free space ..." << std::endl;
                break;
            }
            if (it->prev == nullptr) {
                break;
            }
            it = it->prev;
        }

        // In case: we didn't find any free place in the list of chunks.
        // Create a new chunk.
        if (!found) {
            auto next_chunk = new Chunk(chunks);
            chunks = next_chunk;
            p = reinterpret_cast<T *>(chunks->add_block(n * sizeof(T)));
        }
        return p;

    }

    template<typename T>
    size_t MyAllocator<T>::max_size() {
        return Chunk::CHUNK_SIZE / sizeof(T);
    }

    template<typename T>
    void MyAllocator<T>::deallocate(T *p, size_type n) noexcept {
//    self_report();
//    report(p, n, false); // no actual de-allocation
    }

    template<typename T>
    template<typename ... Args>
    void MyAllocator<T>::construct(T *p, Args &&... args) {
//    self_report();
        new(p) T(args...); // new - placement function
//    std::cout << "T()" << std::endl;
    }

    template<typename T>
    void MyAllocator<T>::destroy(T *p) {
//    self_report();
        p->~T();
//    std::cout << "~T()" << std::endl;
    }

    template<typename T>
    MyAllocator<T>::~MyAllocator() {
//    self_report();
//    std::cout << "~MyAllocator()\n";
        if (MyAllocator::copies == 1) {
            while (true) {
                auto prev = chunks->prev;
                delete chunks;
                if (prev == nullptr) {
                    break;
                }
                chunks = prev;
            }
//        std::cout << "~ !!! DESTRUCTED\n";
        } else {
//        std::cout << "~ no actual destructor\n";
        }
        MyAllocator::copies -= 1;
    }
}


#endif //HW_5_ALLOCATOR_MAIN_H
