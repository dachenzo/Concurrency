#pragma once 
#include <atomic>

namespace Cx {
    struct SpinMutex {
        SpinMutex();

        SpinMutex(SpinMutex& other) = delete;

        SpinMutex& operator=(SpinMutex& othrt) = delete;

        void lock();

        bool try_lock();

        void unlock();

        private:
        std::atomic_flag flag_;
    };
}