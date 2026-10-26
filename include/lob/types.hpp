#pragma once
#include <cstdint>

namespace lob {
    
    // signed integer to represent both negative and positive prices and price differences (ask - bid) can go negative safely without wrapping
    using Price = std::int64_t;

    // signed integer requires one '> 0' check after whereas unsigned requires checking 'if fill > remaining' before every subtraction
    using Quantity = std::int64_t;

    // unsigned integer as we never subtract OrderId and we increase range of positive IDs
    using OrderId = std::uint64_t;

    // 1 byte is enough to represent a Buy or Sell
    enum class Side : std::uint8_t {Buy, Sell};

    // static price band: orders priced outside [kMinTick, kMaxTick] are rejected (PriceOutOfRange)
    // both engines use the same band; the flat price array needs a fixed window to index into
    inline constexpr Price kMinTick = 1;          // $0.01
    inline constexpr Price kMaxTick = 100'000;    // $1,000.00
}

