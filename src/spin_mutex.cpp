#include <cx/spin_mutex.hpp>
#if defined(__x86_64__) || defined(_M_X64)
  #include <immintrin.h>
  inline void cpu_relax() { _mm_pause(); }
#else
  #include <thread>
  inline void cpu_relax() { std::this_thread::yield(); }
#endif

namespace Cx {
    SpinMutex::SpinMutex(): flag_{} {

    }

    void SpinMutex::lock() noexcept {

        for (;;) {
            
            if (!flag_.test_and_set(std::memory_order_acquire)) return;

            
            while (flag_.test(std::memory_order_relaxed)) {
                // polite spin: PAUSE/backoff
                for (int i = 0; i < 64; ++i) cpu_relax();
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