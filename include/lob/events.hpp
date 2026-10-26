#pragma once
#include <cstdint>
#include <variant>
#include "lob/types.hpp"

namespace lob {
    // why an order or cancel was rejected
    enum class RejectReason : std::uint8_t {
        ZeroQty,        // new order with qty <= 0
        DuplicateId,    // new order reuses the ID of an order still resting in the book
        PriceOutOfRange,// new order's price is outside [MIN_TICK, MAX_TICK]
        UnknownId       // cancel for an ID that isn't resting (never existed, already filled or cancelled)
    };

    // new order passed validation (emitted before any of its trades)
    struct Accepted {
        OrderId id;
        Side side;
        Price price;
        Quantity qty;

        // each event type needs operator overloading for equivalence for testing
        bool operator==(const Accepted&) const = default;
    };

    // new order or cancel failed validation; the book is unchanged
    struct Rejected {
        OrderId id;
        RejectReason reason;

        bool operator==(const Rejected&) const = default;
    };

    // one fill between an incoming order and a resting order, at the resting order's price
    struct Trade {
        OrderId aggressor_id;
        OrderId resting_id;
        Price price;
        Quantity qty;

        bool operator==(const Trade&) const = default;
    };

    // cancel succeeded; qty is what was still resting when it was removed
    struct Cancelled {
        OrderId id;
        Quantity qty;

        bool operator==(const Cancelled&) const = default;
    };

    // std::vector only holds one type of an event, and each event are different sizes
    // hence use std::variant rather than base class + virtual which hinders performance
    // from branch mispredictions flushing pipelines & heap allocations for each event
    using Event = std::variant<Accepted, Rejected, Trade, Cancelled>;
}
