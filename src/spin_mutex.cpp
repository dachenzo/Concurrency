#include <cx/spin_mutex.hpp>
#include <cx/detail.hpp>
#include <algorithm>

namespace Cx {
    SpinMutex::SpinMutex(): flag_{} {

    }

    void SpinMutex::lock() noexcept {

        for (;;) {
            
            if (!flag_.test_and_set(std::memory_order_acquire)) return;

            
            while (flag_.test(std::memory_order_relaxed)) {
                // polite spin: PAUSE/backoff
                std::size_t wait = 32;
                for (std::size_t i = 0; i < wait; ++i) cpu_relax();
                wait = std::min<std::size_t>(1<<15, wait);
                
            }
         
        }
    }
    

    void SpinMutex::unlock() noexcept {
        flag_.clear(std::memory_order_release);
    }

    bool SpinMutex::try_lock() noexcept {
        return !flag_.test_and_set(std::memory_order_acquire);
    }

}