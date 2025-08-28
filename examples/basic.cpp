#include <cx/lock_guard.hpp>
#include <cx/spin_mutex.hpp>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

int main() {
    Cx::SpinMutex m;                 
    std::atomic<int> x{0};

    auto work = [&]{
        for (int i = 0; i < 1000000; ++i) {
            Cx::LockGuard<Cx::SpinMutex> g{m};
            x.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::vector<std::thread> ts(4);
    for (auto& t : ts) t = std::thread(work);
    for (auto& t : ts) t.join();

    std::cout << "x=" << x.load() << "\n";
}
