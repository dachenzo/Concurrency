#pragma once
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <atomic>
#include <stdint.h>

namespace Cx {
    struct FutexMutex {
        
        void lock() {
            int expected = 0;
            if (!state_.compare_exchange_strong(expected, 1 , std::memory_order_acquire)) {
                slow_lock();
            }
        }

        void unlock() {
            state_.store(0, std::memory_order_release);
            syscall(SYS_futex, (int*)&state_, FUTEX_WAKE, 1, NULL, NULL, 0);
        }

        void slow_lock() {
            while (true) {
                int expected = 0;
                if (state_.compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
                    return;
                }
                syscall(SYS_futex, (int*)&state_, FUTEX_WAIT, 1, NULL, NULL, 0);
            }
        }

        private:
        std::atomic<int> state_{0};
    };
}