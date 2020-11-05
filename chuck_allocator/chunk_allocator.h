#ifndef HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H
#define HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H

#define DEBUG  true

#include <cstdlib>
#include <memory>
#include <iostream>
#include <list>
#include <stdexcept>


namespace task {

    /**
     * Responsible for allocating and manage memory.
     */
    class Chunk {
    private:
        uint8_t *p; // pointer to block with CHUNK_SIZE bytes
        size_t size_of_data_memory; // size of block part of spent memory
    public:
        const static size_t CHUNK_SIZE = 100; //bytes


        Chunk() = delete;


        explicit Chunk(size_t size = CHUNK_SIZE) : size_of_data_memory(0) {
            p = new uint8_t[size];
#ifdef DEBUG
            std::cout << "Chunk() at " << std::hex << std::showbase << reinterpret_cast<void *>(p) << std::dec
                      << std::endl;
#endif
        }

        Chunk(const Chunk &other);


        /// Checks the chunk is allocated.
        bool is_empty() const {
#ifdef DEBUG
            if (p == nullptr) {
                std::cout << "Chunk is empty." << std::endl;
            }
#endif
            return (p == nullptr);
        }

        /// Returns number of bytes that can be used for data.
        std::size_t get_size_of_free_memory() const {
            return Chunk::CHUNK_SIZE - size_of_data_memory;
        }

        /// Returns number of bytes that spent for data.
        std::size_t get_size_of_data_memory() const {
            return size_of_data_memory;
        }

        ~Chunk() {
            if (p != nullptr) {
                delete[] static_cast<uint8_t *>(p);
            }
#ifdef DEBUG
            std::cout << "~Chunk()" << std::endl;
#endif
        }
    };

    /**
     * Node in a List.
     */
    template<typename T>
    class Node {
    public:
        T *data; //pointer to data.
        Node *next; // pointer to next Node

        Node() : data(new T()), next(nullptr) {}

    };


    /**
     * Simple list.
     */
    template<typename T>
    class SimpleList {
    public:
        Node<T> *begin; // start node of list
        std::size_t size;


        SimpleList() : begin(nullptr), size(0) {}


        /**
         * Adds another node to the tail
         */
        T *add() {
            auto node = new Node<T>(); // create a node
            if (!begin) {
                begin = node;
            } else {
                auto it = begin;
                while (it->next != nullptr) {
                    it = it->next;
                }
                it->next = node;
            }
            size += 1;
            return node;
        }

        bool is_empty() {
            return (size == 0);
        }


        ~SimpleList() {
            if (begin != nullptr) {
                auto it = begin;
                while (it->next != nullptr) {
                    auto tmp = it->next;
                    delete it;
                    it = tmp;
                }
                delete it; // delete tail
            }
        }
    };


    /**
     Custom Allocator with chunks.
     */
    template<typename T>
    class ChunkAllocator {
    private:
        SimpleList<Chunk> *lst; // list of chunks
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
            typedef ChunkAllocator<U> other;
        };

    public:
        ChunkAllocator() : lst(new SimpleList<T>()) {
#ifdef DEBUG
            self_report();
            std::cout << "ChunkAllocator()\n";
#endif
        };

        ChunkAllocator(const ChunkAllocator &other);

        ChunkAllocator<T> &operator=(ChunkAllocator<T> const &other);

        size_t max_size() {
            return Chunk::CHUNK_SIZE / sizeof(T);
        }

        /**
         * Allocates memory for n element of value_type.
         * @param n number of elements;
         * @return pointer to allocated memory.
         */
        pointer allocate(size_t n) {
#ifdef DEBUG
            self_report();
            std::cout << "Allocating " << n << " items" << std::endl;
#endif

            // Request capacity more than Chunk::CHUNK_SIZE
            if (n > max_size()) {
                std::cout << "Request amount of memory more than " << Chunk::CHUNK_SIZE << " bytes." << std::endl;
                throw std::bad_alloc();
            }

            // If list of begin_of_list is empty add chunk.
            if (lst->is_empty()) {
                lst->add();
            }

            bool found = false;

            // Add n elements. Go through list of begin_of_list to find free place
            // for requested amount of memory.
            auto it = lst->begin;
            auto chunk = it->data;
            while (true) {
                if (chunk->get_size_of_free_memory() >= n * sizeof(T)) {
                    p = reinterpret_cast<T *>(chunk->add_block(n * sizeof(T)));
                    found = true;
//                std::cout << "Found free space ..." << std::endl;
                    break;
                }
                if (it->prev == nullptr) {
                    break;
                }
                it = it->prev;
            }
        }


        void deallocate(T *p, size_type n) noexcept;

        template<typename ... Args>
        void construct(T *p, Args &&... args);

        void destroy(T *p);

        ~ChunkAllocator();

    private:

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



// Copy constructor
//    template<typename T>
//    ChunkAllocator<T>::ChunkAllocator(const ChunkAllocator &other) {
////    self_report();
////    std::cout << "copy ChunkAllocator()\n";
//        if (this != &other) {
//            ChunkAllocator::copies += 1;
//            delete this->start; // Destruct previous version
//            this->start = other.start;
//        }
//    };

//    template<typename T>
//    ChunkAllocator<T> &ChunkAllocator<T>::operator=(const ChunkAllocator<T> &other) {
////    self_report();
////    std::cout << "copy ChunkAllocator()\n";
//        if (this != &other) {
//            ChunkAllocator::copies += 1;
//            delete this->start; // Destruct previous version of begin_of_list
//            this->start = other.start;
//        }
//        return *this;
//    }
//
//    template<typename T>
//    typename ChunkAllocator<T>::
//
//    // In case: we didn't find any free place in the list of begin_of_list.
//    // Create a new chunk.
//    if (!found) {
//    auto next_chunk = new Chunk(start);
//    start = next_chunk;
//    p = reinterpret_cast<T *>(start->add_block(n * sizeof(T)));
//}
//return
//p;
//
//}


    template<typename T>
    void ChunkAllocator<T>::deallocate(T *p, size_type n) noexcept {
//    self_report();
//    report(p, n, false); // no actual de-allocation
    }

    template<typename T>
    template<typename ... Args>
    void ChunkAllocator<T>::construct(T *p, Args &&... args) {
//    self_report();
        new(p) T(args...); // new - placement function
//    std::cout << "T()" << std::endl;
    }

    template<typename T>
    void ChunkAllocator<T>::destroy(T *p) {
//    self_report();
        p->~T();
//    std::cout << "~T()" << std::endl;
    }

    template<typename T>
    ChunkAllocator<T>::~ChunkAllocator() {
//    self_report();
//    std::cout << "ChunkAllocatortor()\n";
        if (ChunkAllocator::copies == 1) {
            while (true) {
                auto prev = start->prev;
                delete start;
                if (prev == nullptr) {
                    break;
                }
                start = prev;
            }
//        std::cout << "~ !!! DESTRUCTED\n";
        } else {
//        std::cout << "~ no actual destructor\n";
        }
        ChunkAllocator::copies -= 1;
    }

}


#endif //HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H
