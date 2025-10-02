#include <new>
#include <type_traits>
#include <concepts>
#include <cx/detail.hpp>
#include <cstddef>



namespace Cx {

    template<typename M>
    concept Lockable = requires (M m) {
        {m.lock()} -> std::same_as<void>;
        {m.unlock()} -> std::same_as<void>;
        {m.try_lock()} -> std::same_as<void>;
    };

    template <Lockable Mutex>
    struct alignas(Cx::hw_destructive_size) PaddedLockable: Mutex {

        using Mutex::Mutex;
        using Mutex::lock;
        using Mutex::unlock;
        using Mutex::try_lock;

        private:
        static constexpr std::size_t line = Cx::hw_destructive_size;
        static constexpr std::size_t rem = sizeof(M) % line;
        static constexpr std::size_t pad = rem ? line - rem : 0;


        char __pad__[pad ? pad : line]; // bad possibly wasting  a cache line
    };
}




