#include "MemoryPool.h"
#include <iostream>

class T {
public:
    T() {
        std::cout << "Call from constructor T()" << "\n";
    }

    ~T() {
        std::cout << "Call from destructor ~T()" << "\n";
    }
};

class TP {
public:
    TP() {
        std::cout << "Call from constructor TP()" << "\n";
    }

    ~TP() {
        std::cout << "Call from destructor ~TP()" << "\n";
    }
};

int main() {
    {
        MemoryPool<T> pool(4);

        auto* o1 = pool.alloc();
        auto* o2 = pool.alloc();

        pool.free(o1);
        pool.free(o2);
    }

    {
        MemoryPool<TP> pool(4);

        auto* o1 = pool.alloc();
        auto* o2 = pool.alloc();

        pool.free(o1);
        pool.free(o2);
    }
	return 0;
}