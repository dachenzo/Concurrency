#pragma once
#include <cstddef>
#include <cstdlib>


namespace Cx {
    #if defined(__x86_64__) || defined(_M_X64)
        #include <immintrin.h>
        inline void cpu_relax() { _mm_pause(); }
    #elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
        #include <arm_acle.h>
        inline void cpu_relax() { __yield(); }
    #else
        #include <thread>
        inline void cpu_relax() { std::this_thread::yield(); }
    #endif



    
    #if defined(__cpp_lib_hardware_interference_size)
        inline constexpr std::size_t hw_destructive_size = std::hardware_destructive_interference_size;

        inline constexpr std::size_t hw_constructive_size = std::hardware_constructive_interference_size;
    #else 
        inline constexpr std::size_t hw_destructive_size = 64;
        inline constexpr std::size_t hw_constructive_size = 64;
    #endif

};