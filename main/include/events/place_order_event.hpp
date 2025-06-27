#ifndef PLACE_ORDER_EVENT_HPP 
#define PLACE_ORDER_EVENT_HPP

struct PlaceOrderEvent {
	int orderID;

	explicit PlaceOrderEvent(int id): orderID(id) {};
};

#ifndef NO_LOGGING
// Formats the event log.
std::ostream& operator<<(std::ostream &out, const PlaceOrderEvent& event) {
    out  << "orderID: " << event.orderID;
    return out;
}
#endif

#endif // PLACE_ORDER_EVENT_HPP
