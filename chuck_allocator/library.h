#ifndef HW_5_ALLOCATOR_LIBRARY_H
#define HW_5_ALLOCATOR_LIBRARY_H

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
        uint copies;
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

}

#endif //HW_5_ALLOCATOR_LIBRARY_H
