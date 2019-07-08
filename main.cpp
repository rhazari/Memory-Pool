#include "MemoryPool.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>

#define SIMULATION_RUNS 10000

class T {
public:
    T() {
    }

    ~T() {
    }
};


T* alloc(){
    auto* obj = new T();
    return obj;
}

int main() {
    // Case 1: Spinning out 20 threads to allocate memory to 20 objects from the alloc() function
    {
        std::chrono::duration<double> time_elapsed;
        for(auto k = 0; k < SIMULATION_RUNS; ++k){
            std::vector<T*> vec;
            auto start = std::chrono::high_resolution_clock::now();
            for(auto i = 0; i < 20; ++i){
                std::future<T*> obj = std::async(std::launch::async, alloc);
                vec.push_back(obj.get());
            }
            auto end = std::chrono::high_resolution_clock::now();
            time_elapsed += end - start;

            for(auto obj: vec){
                delete obj;
            }
        }
        time_elapsed /= SIMULATION_RUNS;
        std::cout<<"Avg time for creating 20 objects without using Memory-Pool:            "<< time_elapsed.count()<<"\n";
    }
    // Case 2: Creating a thread pool with 20 threads
    // Then utilizing the pool for allocating memory to 20 objects
    {
        MemoryPool<T> pool(20);

        std::chrono::duration<double> time_elapsed;
        for(auto k = 0; k < SIMULATION_RUNS; ++k){
            std::vector<T*> vec;
            auto start = std::chrono::high_resolution_clock::now();
            for(auto i = 0; i < 20; ++i){
                auto* obj = pool.alloc();
                vec.push_back(obj);
            }

            auto end = std::chrono::high_resolution_clock::now();

            time_elapsed += end - start;
            for(auto obj: vec){
                pool.free(obj);
            }
        }
        time_elapsed /= SIMULATION_RUNS;
        std::cout<<"Avg time for creating 20 objects when using Memory-Pool(threads = 20): "<< time_elapsed.count()<<"\n";
    }
    // Case 3: Creating a thread pool with 10 threads
    // Then utilizing the pool for allocating memory to 20 objects
    {
        MemoryPool<T> pool(10);

        std::chrono::duration<double> time_elapsed;
        for(auto k = 0; k < SIMULATION_RUNS; ++k){
            std::vector<T*> vec;
            auto start = std::chrono::high_resolution_clock::now();
            for(auto i = 0; i < 20; ++i){
                auto* obj = pool.alloc();
                vec.push_back(obj);
            }

            auto end = std::chrono::high_resolution_clock::now();

            time_elapsed += end - start;
            for(auto obj: vec){
                pool.free(obj);
            }
        }
        time_elapsed /= SIMULATION_RUNS;
        std::cout<<"Avg time for creating 20 objects when using Memory-Pool(threads = 10): "<< time_elapsed.count()<<"\n";
    }
	return 0;
}
