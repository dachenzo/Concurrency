#include <cx/spin_mutex.hpp>


namespace Cx {
    SpinMutex::SpinMutex(): flag_{} {

    }

    void SpinMutex::lock() {

        while(flag_.test_and_set(std::memory_order_acquire)) {

        }
    }

    void SpinMutex::unlock() {
        flag_.clear(std::memory_order_release);
    }

    bool SpinMutex::try_lock() {
        return !flag_.test_and_set(std::memory_order_acquire);
    }

}