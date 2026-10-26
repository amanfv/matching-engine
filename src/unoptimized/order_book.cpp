#include "lob/unoptimized/order_book.hpp"

namespace lob::unoptimized {

    // skeleton
    void OrderBook::add(OrderId id, Side side, Price price, Quantity qty, std::vector<Event>& out) {
        (void)id; (void)side; (void)price; (void)qty; (void)out;
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
