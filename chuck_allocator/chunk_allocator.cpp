#include <iostream>
#include <vector>

#include "main.h"

using namespace task;

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

    std::cout << "\nclass A\n";
    MyAllocator<A> a_aloc;
    auto p = a_aloc.allocate(1);
    a_aloc.construct(p, 1, 2);
    a_aloc.destroy(p);
    a_aloc.deallocate(p, 1);
    p = a_aloc.allocate(1);
    a_aloc.construct(p, 1, 2);
    a_aloc.destroy(p);
    a_aloc.deallocate(p, 1);

    std::cout << "\nVectors\n";
    std::vector<A, MyAllocator<A>> vector1(5);
    auto a1 = A(2, 4);
    vector1.emplace_back(a1);

}




