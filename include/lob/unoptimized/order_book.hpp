#pragma once
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>
#include "lob/events.hpp"

namespace lob::unoptimized {

    // resting order (price and side are obvious by Level/map it lives in)
    struct Order {
        OrderId id;
        Quantity qty;       // remaining qty, drops on partial fills
    };

    // FIFO queue of resting orders and total quantity of orders
    struct Level {
        std::list<Order> queue;
        Quantity total_qty = 0;     
    };

    // where an order lives so cancel can reach it from just an ID
    // side + price find the Level, then finds order within queue via iterator
    struct Locator {
        Side side;
        Price price;
        std::list<Order>::iterator it;
    };

    class OrderBook {
    public:
        // new limit order: validate, match against the other side, rest any remainder, append events performed to out
        void add(OrderId id, Side side, Price price, Quantity qty, std::vector<Event>& out);

        // remove a resting order by ID (reference to std::vector<Event> so heap allocations responsible to caller)
        void cancel(OrderId id, std::vector<Event>& out);

        // best prices; empty if that side of the book has no orders
        std::optional<Price> best_bid() const;
        std::optional<Price> best_ask() const;

    private:
        std::map<Price, Level> asks_;                       // low -> high, begin() = best ask
        std::map<Price, Level, std::greater<Price>> bids_;  // high -> low, begin() = best bid
        std::unordered_map<OrderId, Locator> ids_;          // for cancelling orders w/ OrderId 
    };
}
