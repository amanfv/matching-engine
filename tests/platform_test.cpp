#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "lob/platform.hpp"

// Compile time check for kCacheLine being a power of 2
// x & (x - 1): 64: 1000000, 63: 0111111 -> 64 & 63 must be 0
static_assert((lob::kCacheLine & (lob::kCacheLine - 1)) == 0, "kCacheLine required to be a power of 2");

// Runtime check for cache line size
TEST(Platform, CacheLineSize) {
#if defined(__x86_64__)
    EXPECT_EQ(lob::kCacheLine, 64u);
#elif defined(__aarch64__)
    EXPECT_EQ(lob::kCacheLine, 128u);
#endif
}

// Counter must move forward over time (compare w/ GT)
TEST(Platform, NowTicksAdvances) {
    auto first = lob::now_ticks();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto second = lob::now_ticks();
    EXPECT_GT(second, first);
}

// Linux: after pinning to CPU 1 we must actually be running on CPU 1
// macOS: pinning isn't supported, so it must report false
TEST(Platform, PinToCpu) {
#if defined(__linux__)
    ASSERT_TRUE(lob::pin_to_cpu(1));
    EXPECT_EQ(sched_getcpu(), 1);
#else
    EXPECT_FALSE(lob::pin_to_cpu(1));
#endif
}
