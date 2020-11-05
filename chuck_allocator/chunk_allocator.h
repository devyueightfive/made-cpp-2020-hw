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
    template<size_t N>
    class Chunk {
    private:
        uint8_t *p; // pointer to block with CHUNK_SIZE bytes
        size_t _size_of_data_memory;// size of block part of spent memory
    public:

        explicit Chunk() : _size_of_data_memory(0) {
            p = new uint8_t[N];
#ifdef DEBUG
            std::cout << "Chunk() at " << std::hex << std::showbase << reinterpret_cast<void *>(p) << std::dec
                      << std::endl;
#endif
        }

        Chunk(const Chunk &other);


        uint8_t *add_block(std::size_t n) {
            if (n > get_size_of_free_memory()) {
#ifdef DEBUG
                std::cout << "Chunk: requested more bytes than defined in constructor " << N << " while your n is ("
                          << n << ")" << std::endl;
#endif
                throw std::bad_alloc();
            }
            auto res = p + this->_size_of_data_memory;
            this->_size_of_data_memory += n;
            return res;
        }


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
            return N - this->_size_of_data_memory;
        }

        /// Returns number of bytes that spent for data.
        std::size_t get_size_of_data_memory() const {
            return this->_size_of_data_memory;
        }

        std::size_t max_size() const {
            return N;
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
        Node<T> *add() {
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
        static const long MAX_BYTES = 100; //bytes
        SimpleList<Chunk<MAX_BYTES>> *lst; // list of chunks
//        static uint copies;

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
        ChunkAllocator() : lst(new SimpleList<Chunk<MAX_BYTES>>()) {
#ifdef DEBUG
            self_report("ChunkAllocator()");
#endif
        };

        ChunkAllocator(const ChunkAllocator &other);

        ChunkAllocator<T> &operator=(ChunkAllocator<T> const &other);

        size_t max_size() {
            return MAX_BYTES / sizeof(T);
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
            if (n > max_size()) { // Request capacity more than Chunk::CHUNK_SIZE
                std::cout << "Request amount of memory more than " << MAX_BYTES << " bytes." << std::endl;
                throw std::bad_alloc();
            }
            if (lst->is_empty()) {// If list of chunks is empty add chunk.
                lst->add();
            }
            bool found = false; // indicates free space in existed chunks
            pointer res; // pointer to result
            /**
             * Add n elements. Pass through the list to find free place in existed chunks.
            */
            auto it = lst->begin;
            auto chunk = it->data;
            while (true) {
                if (n * sizeof(T) < chunk->get_size_of_free_memory()) {
                    res = reinterpret_cast<T *>(chunk->add_block(n * sizeof(T)));
                    found = true;
                    break;
                }
                if (it->next == nullptr) {
                    break;
                }
                it = it->next;
                chunk = it->data;
            }
            /**
             * In case: we didn't find any free place in existed chunks. Create a new chunk.
             */
            if (!found) {
                auto last_node = lst->add();
                chunk = last_node->data;
                res = reinterpret_cast<T *>(chunk->add_block(n * sizeof(T)));
            }
            return res;
        } // allocate()


        void deallocate(T *p, size_type n) noexcept {
#ifdef DEBUG
            self_report();
            report(p, n, false); // no actual de-allocation
#endif
        }


        template<typename ... Args>
        void construct(T *p, Args &&... args) {
#ifdef DEBUG
            self_report("construct ChunkAllocator");
#endif
            new(p) T(args...); // new - placement function
        }

        void destroy(T *p) {
#ifdef DEBUG
            self_report("destroy ChunkAllocator");
#endif
            p->~T();
        }

        ~ChunkAllocator() {
#ifdef DEBUG
            self_report("~ChunkAllocator()");
#endif
//            if (ChunkAllocator::copies == 1) {
//
//#ifdef DEBUG
//                std::cout << "~ !!! DESTRUCTED\n";
//#endif
//            } else {
//#ifdef DEBUG
//                std::cout << "~ no actual destructor\n";
//#endif
//            }
//            ChunkAllocator::copies -= 1;
        }

    private:

        void report(T *p, size_type n, bool alloc = true) {
            std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                      << " bytes at " << std::hex << std::showbase
                      << reinterpret_cast<void *>(p) << std::dec << std::endl;
        }


        void self_report(const char *text = "") {
            std::cout << "i'm : " << std::hex << std::showbase
                      << reinterpret_cast<void *>(this) << std::dec
                      //                      << " copy is " << this->copies
                      << std::endl;
            std::cout << text << std::endl;
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








} // namespace task


#endif //HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H
