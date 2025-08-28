#pragma once 
#include <concepts>
#include <type_traits>

namespace Cx {

    template<typename M>
    concept BasicLockable = requires(M m) {
        {m.lock()} -> std::same_as<void>;
        {m.unlock()} -> std::same_as<void>;
    };

    template<BasicLockable Mutex>
    struct LockGuard {

        explicit  LockGuard(Mutex& m) noexcept: m_{m} {
            m_.lock();
        }

        ~LockGuard() noexcept {
            m_.unlock();
        }

        private:
        Mutex& m_;

    };


}