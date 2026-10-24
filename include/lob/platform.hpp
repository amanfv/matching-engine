#pragma once
#include <chrono>
#include <cstddef>
#include <cstdint>

#if defined(__x86_64__)
    #include <x86intrin.h>  // __rdtsc(): CPU time stamp counter on x86
#endif

#if defined(__linux__)
    #include <pthread.h>    // pthread_setaffinity_np, pthread_self
    #include <sched.h>      // cpu_set_t, CPU_ZERO, CPU_SET
#endif

namespace lob {

    // constexpr: value is known at compilation time
    // inline: keeps one definition of kCacheLine and treats it as a single shared variable across all files that include platform.hpp
    #if defined(__x86_64__)
        inline constexpr std::size_t kCacheLine = 64;
    #elif defined(__aarch64__)
        inline constexpr std::size_t kCacheLine = 128;
    #else
        #error "Unsupported architecture: define kCacheLine for the CPU"
    #endif

    // cheap timestamp for latency measurement
    // x86: raw TSC ticks (fixed rate, not CPU cycles)
    // ARM: nanoseconds from steady_clock (~42 ns resolution)
    inline std::uint64_t now_ticks() {
    #if defined(__x86_64__)
        return __rdtsc();
    #else
        auto t = std::chrono::steady_clock::now().time_since_epoch();
        return static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(t).count());
    #endif
    }

    // pin the thread to one CPU so OS scheduler can't move it
    // - prevents the thread from switching to another core which would cause cold misses
    // - does NOT stop other threads from running on the core which can evict cache lines
    //   which requires an idle core (unable to access root on ug machines)
    // Linux: returns true if pinned
    // macOS: no pinning API, so does nothing and returns false
    inline bool pin_to_cpu(int cpu) {
    #if defined(__linux__)
        cpu_set_t set;          // bitmask: one bit per CPU
        CPU_ZERO(&set);         // clear all bits
        CPU_SET(cpu, &set);     // allow only `cpu`
        return pthread_setaffinity_np(pthread_self(), sizeof(set), &set) == 0;
    #else
        (void)cpu;              // silence the unused-parameter warning
        return false;
    #endif
    }
}
