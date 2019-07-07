#pragma once
#include <condition_variable>
#include <functional>
#include <vector>
#include <future>
#include <thread>
#include <queue>


template <class ELEM_TYPE>
class MemoryPool {
public:
    // Pre-allocate a thread pool with a fixed number of threads
    explicit MemoryPool(std::size_t numThreads) {
        start(numThreads);
    }

    // Terminate the thead pool. Join all the threads in the pool
    ~MemoryPool() {
        stop();
    }

    // Return a pointer to a newly created object
    ELEM_TYPE * alloc() {
        auto f = enqueue([=] {
            auto* ptr = new ELEM_TYPE();
            return ptr;
        });
        return f.get();
    }

    // Delete the object
    void free(ELEM_TYPE* ptr) {
        auto f = enqueue([=] {
            delete ptr;
        });
        f.get();
    }

private:
    std::vector<std::thread> m_threads;
    std::condition_variable m_eventVar;
    std::mutex m_mutex;
    std::queue<std::function<void()>> m_queue;
    bool stopFlag = false;

    template<class T>
    auto enqueue(T task) -> std::future<decltype(task())> {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_queue.emplace([=] {
                (*wrapper)();
            });
        }
        m_eventVar.notify_one();
        return wrapper->get_future();
    }

    void start(int numThreads) {
        for (auto k = 0; k < numThreads; ++k) {
            m_threads.emplace_back([=] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock{ m_mutex };
                        m_eventVar.wait(lock, [=] {
                            return stopFlag || !m_queue.empty();
                        });

                        if (stopFlag && m_queue.empty())
                            break;

                        task = m_queue.front();
                        m_queue.pop();
                    }
                    task();
                }
            });
        }
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            stopFlag = true;
        }
        m_eventVar.notify_all();

        for (auto& thread : m_threads) {
            thread.join();
        }
    }
};