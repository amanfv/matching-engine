#include "lob/unoptimized/order_book.hpp"
#include <iterator>

namespace lob::unoptimized {
    
    // validate order before adding to LOB
    void OrderBook::add(OrderId id, Side side, Price price, Quantity qty, std::vector<Event>& out) {
        if (qty <= 0) {
            out.push_back(Rejected{id, RejectReason::ZeroQty});
            return;
        } else if (price < kMinTick || price > kMaxTick) {
            out.push_back(Rejected{id, RejectReason::PriceOutOfRange});
            return;
        } else if (ids_.contains(id)) {
            out.push_back(Rejected{id, RejectReason::DuplicateId});
            return;
        } else {
            out.push_back(Accepted{id, side, price, qty});
        }

        // unable to define 'level' above as references cannot be repointed, so define iterator here
        std::list<Order>::iterator it;

        // get the level by reference to avoid modifying a copy
        if (side == Side::Buy) {
            Level& level = bids_[price];
            
            level.queue.push_back(Order{id, qty});
            level.total_qty += qty;

            // assign iterator to last element we just added (.end() is one past last element)
            it = std::prev(level.queue.end());     

        } else {
            Level& level = asks_[price];

            level.queue.push_back(Order{id, qty});
            level.total_qty += qty;
            it = std::prev(level.queue.end());
        }

        ids_[id] = Locator{side, price, it};
    }

    // skeleton
    void OrderBook::cancel(OrderId id, std::vector<Event>& out) {
        (void)id; (void)out;
    }

    // bids_ is sorted high -> low, so the first key is the highest bid
    std::optional<Price> OrderBook::best_bid() const {
        if (bids_.empty())
            return std::nullopt;    // nullopt is the nullptr for optionals

        // returns map key in the std::pair that std::map stores (the price)
        return bids_.begin()->first;
    }

    // asks_ is sorted low -> high, so the first key is the lowest ask
    std::optional<Price> OrderBook::best_ask() const {
        if (asks_.empty())
            return std::nullopt;

        return asks_.begin()->first;
    }
}
