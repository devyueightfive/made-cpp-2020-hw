#include <iostream>
#include <stdexcept>
#include "library.h"

using namespace task;

Chunk::Chunk() : p(nullptr), size_of_data(0), copies(1), prev(nullptr) {
    std::cout << "Chunk()" << std::endl;
};

Chunk::Chunk(Chunk *prev, size_t size = Chunk::CHUNK_SIZE) : size_of_data(0), prev(prev) {
    p = new uint8_t[size];
    std::cout << "Chunk() at " << std::hex << std::showbase
              << reinterpret_cast<void *>(p) << std::dec << std::endl;
}

void Chunk::allocate(size_t size = CHUNK_SIZE) {
    p = new uint8_t[size];
    std::cout << "Chunk() at " << std::hex << std::showbase
              << reinterpret_cast<void *>(p) << std::dec << std::endl;
}


Chunk::Chunk(const Chunk &other) {
    std::cout << "Copy Chunk() at " << std::hex << std::showbase
              << reinterpret_cast<void *>(p) << std::dec << std::endl;
    if (this != &other) {
        delete[] static_cast<uint8_t *>(p);
        p = other.p;
        size_of_data = other.size_of_data;
//            this->copies += 1;
    }
}

bool Chunk::is_empty() {
    if (p == nullptr) {
        std::cout << "Chunk is empty." << std::endl;
    }
    return (p == nullptr);
}

size_t Chunk::get_free_size() const {
    return Chunk::CHUNK_SIZE - size_of_data;
}

uint8_t *Chunk::add_block(std::size_t n) {
    if (n > get_free_size()) {
        std::cout << "Chunk: requested more bytes than defined in CHUNK_SIZE while your n is (" << n
                  << ")" << std::endl;
        throw std::bad_alloc();
    }
    auto return_pointer = p + size_of_data;
    size_of_data += n;
    std::cout << "New block at " << std::hex << std::showbase
              << reinterpret_cast<void *>(return_pointer)
              << " ends in " << reinterpret_cast<void *>(p + size_of_data) << std::dec
              << " with size " << n << " bytes" << std::endl;
    return return_pointer;
}


Chunk::~Chunk() {
//        if (this->copies == 1) {
    delete[] static_cast<uint8_t *>(p);
//        } else {
//            this->copies -= 1;
//        }
    std::cout << "~Chunk()" << std::endl;
}

template<typename T>
uint MyAllocator<T>::copies = 1;

template<typename T>
MyAllocator<T>::MyAllocator() {
    chunks = new Chunk();
    self_report();
    std::cout << "MyAllocator()\n";
};

// Copy constructor
template<typename T>
MyAllocator<T>::MyAllocator(const MyAllocator &other) {
    self_report();
    std::cout << "copy MyAllocator()\n";
    if (this != &other) {
        MyAllocator::copies += 1;
        delete this->chunks; // Destruct previous version
        this->chunks = other.chunks;
    }
};

template<typename T>
T *MyAllocator<T>::allocate(size_t n) {
    self_report();
    std::cout << "Allocating " << n << " items" << std::endl;

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
    self_report();
    report(p, n, false); // no actual de-allocation
}

template<typename T>
template<typename ... Args>
void MyAllocator<T>::construct(T *p, Args &&... args) {
    self_report();
    new(p) T(args...); // new - placement function
    std::cout << "T()" << std::endl;
}

template<typename T>
void MyAllocator<T>::destroy(T *p) {
    self_report();
    p->~T();
    std::cout << "~T()" << std::endl;
}

template<typename T>
MyAllocator<T>::~MyAllocator() {
    self_report();
    std::cout << "~MyAllocator()\n";
    if (MyAllocator::copies == 1) {
        while (true) {
            auto prev = chunks->prev;
            delete chunks;
            if (prev == nullptr) {
                break;
            }
            chunks = prev;
        }
    }
    MyAllocator::copies -= 1;
}
// Tests

class A {
    int x;
    int y;
public:
    A(int x, int y) : x(x), y(y) { std::cout << "A(" << x << "," << y << ")" << std::endl; }

    A() {
        x = 0;
        y = 0;
        std::cout << "A(" << x << "," << y << ")" << std::endl;
    }

    ~A() { std::cout << "~A" << std::endl; }
};

int main() {
    task::MyAllocator<int> alloc;
    size_t n1 = 10;
    int *p1 = alloc.allocate(n1);
    size_t n2 = 25;
    int *p2 = alloc.allocate(n2);
    size_t n3 = 10;
    int *p3 = alloc.allocate(n3);
    size_t n4 = 10;
    int *p4 = alloc.allocate(n4);
    alloc.deallocate(p1, n1);
    alloc.deallocate(p3, n3);
    alloc.deallocate(p2, n2);
    alloc.deallocate(p4, n4);

//    cout << "\nclass A\n";
//    MyAllocator<A> alloc;
//    auto p = alloc.allocate(1);
//    alloc.construct(p, 1, 2);
//    alloc.destroy(p);
//    alloc.deallocate(p, 1);
//    p = alloc.allocate(1);
//    alloc.construct(p, 1, 2);
//    alloc.destroy(p);
//    alloc.deallocate(p, 1);

//    cout << "\nVectors\n";
//    std::vector<A, MyAllocator<A>> vector1(5);
//    auto a1 = A(2, 4);
//    vector1.emplace_back(a1);

}




