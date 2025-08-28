#include <new>
#include <type_traits>
#include <concepts>
#include <cstddef>


#ifndef HW_DIS_INTERF_SIZE
    #ifdef __cp_lib_hardware_interference_size
        #define HW_DIS_INTERF_SIZE std::hardware_destructive_interference_size
    #else
        #define HW_DIS_INTERF_SIZE  64u

    #endif
#endif

namespace Cx {

    template<typename M>
    concept Lockable = requires (M m) {
        {m.lock()} -> std::same_as<void>;
        {m.unlock()} -> std::same_as<void>;
        {m.try_lock()} -> std::same_as<void>;
    };

    template <Lockable Mutex>
    struct alignas(HW_DIS_INTERF_SIZE) PaddedLockable: Mutex {

        using Mutex::Mutex;

        using Mutex::lock;
        using Mutex::unlock;
        using Mutex::try_lock;

        private:
        static constexpr std::size_t line = HW_DIS_INTERF_SIZE;
        static constexpr std::size_t rem = sizeof(M) % line;
        static constexpr std::size_t pad = rem ? line - rem : 0;


        char __pad__[pad ? pad : line]; // bad possibly wasting  a cache line
    };
}




