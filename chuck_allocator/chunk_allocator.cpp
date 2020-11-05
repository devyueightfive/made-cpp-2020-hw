#include "chunk_allocator.h"

#include <iostream>
#include <vector>


using namespace std;

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
    {
        task::ChunkAllocator<int> a1;
        task::ChunkAllocator<int> a2(a1);
        task::ChunkAllocator<int> a3 = a1;
        a3 = a2;
        task::ChunkAllocator<int> a4;
        a3 = a4;
    }
    cout << "\n" << endl;
    {
        cout << "T = int" << endl;
        cout << "declaration" << endl;
        task::ChunkAllocator<int> alloc;
        cout << "allocations" << endl;
        size_t n1 = 10;
        int *p1 = alloc.allocate(n1);
        size_t n2 = 25;
        int *p2 = alloc.allocate(n2);
        size_t n3 = 10;
        int *p3 = alloc.allocate(n3);
        size_t n4 = 10;
        int *p4 = alloc.allocate(n4);
        cout << "deallocations" << endl;
        alloc.deallocate(p1, n1);
        alloc.deallocate(p3, n3);
        alloc.deallocate(p2, n2);
        alloc.deallocate(p4, n4);
    }

    cout << "\n" << endl;
    {
        cout << "T = class A" << endl;
        cout << "declaration" << endl;
        task::ChunkAllocator<A> a_aloc;
        cout << "allocation" << endl;
        auto p = a_aloc.allocate(1);
        a_aloc.construct(p, 1, 2);
        a_aloc.destroy(p);
        a_aloc.deallocate(p, 1);
        cout << "allocations" << endl;
        p = a_aloc.allocate(1);
        a_aloc.construct(p, 1, 2);
        a_aloc.destroy(p);
        a_aloc.deallocate(p, 1);
    }
    cout << "\n" << endl;
    {
        cout << "T = vector" << endl;
        task::ChunkAllocator<A> allocator;
        std::vector<A, decltype(allocator)> vector1(5);
        auto a1 = A(2, 4);
        vector1.push_back(a1);


    }


}




