#pragma once 
#include <atomic>

namespace Cx {
    struct SpinMutex {
        SpinMutex();

        SpinMutex(SpinMutex& other) = delete;

        SpinMutex& operator=(SpinMutex& othrt) = delete;

        void lock() noexcept;

        bool try_lock() noexcept;

        void unlock() noexcept;

        private:
        std::atomic_flag flag_;
    };
}