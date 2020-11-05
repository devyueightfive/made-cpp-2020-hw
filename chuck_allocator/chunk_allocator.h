#ifndef HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H
#define HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H

#define DEBUG  true

#include <cstdlib>
#include <memory>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>


namespace task {

    /**
     * Responsible for allocating and manage memory.
     */
    template<size_t N>
    class Chunk {
    private:
        uint8_t *p; // pointer to block with CHUNK_SIZE bytes
        size_t index;// size of block part of spent memory
    public:

        explicit Chunk() : index(0) {
            p = new uint8_t[N];
#ifdef DEBUG
            std::cout << "Chunk CONSTRUCTED at " << this << " points to " << std::hex << std::showbase
                      << reinterpret_cast<void *>(p) << std::dec << std::endl;
#endif
        }

        Chunk(const Chunk &other) {
#ifdef DEBUG
            std::cout << "Chunk : copy-constructed at " << this << std::endl;
#endif
            if (this != &other) {
            }
        };

        /// Returns number of bytes that can be used for data.
        std::size_t get_size_of_free_memory() const {
            return N - this->index;
        }

        /// Is allocatable.
        bool can_allocate(size_t bytes) const {
            return (get_size_of_free_memory() >= bytes);
        }

        /// Returns pointer to allocated place.
        uint8_t *allocate(std::size_t n) {

            if (!can_allocate(n)) {
#ifdef DEBUG
                std::cout << "Chunk: requested more bytes than defined in constructor " << N << " while your n is ("
                          << n << ")" << std::endl;
#endif
                throw std::bad_alloc();
            }
            auto res = p + this->index;
            this->index += n;
#ifdef DEBUG
            std::cout << "Chunk at " << this << " allocate" << " : " << std::hex << std::showbase
                      << reinterpret_cast<void *>(res) << std::dec << std::endl;
#endif
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


        std::size_t max_size() const {
            return N;
        }

        ~Chunk() {
#ifdef DEBUG
            std::cout << "~Chunk() at" << this << std::endl;
#endif
            if (p != nullptr) {
#ifdef DEBUG
                std::cout << "~Chunk pointer to " << std::hex << std::showbase << reinterpret_cast<void *>(p)
                          << std::dec << std::endl;
#endif
                delete[] static_cast<uint8_t *>(p);
            }
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


        SimpleList() : begin(nullptr), size(0) {
#ifdef DEBUG
            std::cout << "SimpleList() at" << this << std::endl;
#endif
        }


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
#ifdef DEBUG
            std::cout << "~SimpleList() at" << this << std::endl;
#endif
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
        explicit ChunkAllocator() : lst(new SimpleList<Chunk<MAX_BYTES>>()) {
#ifdef DEBUG
            self_report("constructor ChunkAllocator()");
#endif
        };

        ChunkAllocator(const ChunkAllocator &other) {
#ifdef DEBUG
            self_report("copy-constructor ChunkAllocator()");
#endif
            if (this != &other) {
//                ChunkAllocator::copies += 1;
                this->lst = other.lst;
            }
        }

        ChunkAllocator &operator=(ChunkAllocator const &other);

        size_t max_size() {
            return MAX_BYTES / sizeof(T);
        }

        /**
         * Allocates memory for n element of value_type.
         * @param n number of elements;
         * @return pointer to allocated memory.
         */
        pointer allocate(size_type n) {
#ifdef DEBUG
            self_report("Allocating items");
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
                    res = reinterpret_cast<T *>(chunk->allocate(n * sizeof(T)));
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
                res = reinterpret_cast<T *>(chunk->allocate(n * sizeof(T)));
            }
            return res;
        } // allocate()


        void deallocate(pointer p, size_type n) noexcept {
#ifdef DEBUG
            self_report("Allocator : deallocate");
#endif
        }


        template<typename ... Args>
        void construct(pointer p, Args &&... args) {
#ifdef DEBUG
            self_report("construct ChunkAllocator");
#endif
            new(p) T(args...); // new - placement function
        }

        void destroy(pointer p) {
#ifdef DEBUG
            self_report("destroy ChunkAllocator");
#endif
            p->~T();
        }

        ~ChunkAllocator() {
#ifdef DEBUG
            self_report("~ChunkAllocator()");
#endif
            delete this->lst;
        }

    private:

        void report(pointer p, size_type n, bool alloc = true) {
            std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                      << " bytes at " << std::hex << std::showbase
                      << reinterpret_cast<void *>(p) << std::dec << std::endl;
        }


        void self_report(const char *text = "") {
            std::cout << "i'm at " << this << " : " << text << std::endl;
        }
    };


    template<typename T>
    ChunkAllocator<T> &ChunkAllocator<T>::operator=(const ChunkAllocator<T> &other) {
#ifdef DEBUG
        std::cout << "copy-operator ChunkAllocator at " << this << std::endl;
#endif
        if (this != &other) {
//            ChunkAllocator::copies += 1;
//            if (this->lst != nullptr) {
//                delete this->lst; // Destruct previous version of begin_of_list
//            }
            this->lst = other.lst;
        }
        return *this;
    }

} // namespace task


#endif //HW_5_ALLOCATOR_CHUNK_ALLOCATOR_H
