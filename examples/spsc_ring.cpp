#include <cx/spsc_ring.hpp>
#include <thread>
#include <iostream>
#include <cx/detail.hpp>
#include <atomic>




int main() {
    Cx::SpscRing<int, 8> q; // effective capacity = 7

    std::atomic<bool> start{false};
    std::atomic<int>  produced{0};
    std::atomic<int>  consumed{0};

    auto prod = [&] {
        // wait for consumer to be ready (optional)
        while (!start.load(std::memory_order_acquire)) {}

        for (int i = 0; i < 20; ++i) {
            // keep trying until it goes in
            while (!q.write(i)) {
                Cx::cpu_relax();
            }
            ++produced;
            
        }
    };

    auto cons = [&] {
        start.store(true, std::memory_order_release);
        while (consumed.load(std::memory_order_relaxed) < 20) {
            if (auto v = q.read()) {
                ++consumed;
                
            } else {
                Cx::cpu_relax();
            }
        }
    };

    std::thread t1(prod), t2(cons);
    t1.join(); t2.join();

    std::cout << "produced: " << produced << '\n';
    std::cout << "consumed: " << consumed << '\n';
   

        




}