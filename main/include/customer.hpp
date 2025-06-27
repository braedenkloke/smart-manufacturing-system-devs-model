#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "event.hpp"

using namespace cadmium;

struct CustomerState {
	std::vector<int> orders;
	bool hasOrders;
	int indexOfNextOrder;
	int timeOfNextOrder;
	int timeOfPrevOrder;

    explicit CustomerState(): hasOrders(false), indexOfNextOrder(0), timeOfNextOrder(0), timeOfPrevOrder(0) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const CustomerState& state) {
    out  << "{ hasOrders: " << state.hasOrders << ", timeOfNextOrder: " << state.timeOfNextOrder << " }";
    return out;
}
#endif

// Atomic DEVS model of a Customer who places orders to the system.
class Customer : public Atomic<CustomerState> {
public:
	Port<Event> placeOrderEventPort;

    // Constructor.
    Customer(const std::string id, std::vector<int> orders) : Atomic<CustomerState>(id, CustomerState()) {
		placeOrderEventPort = addOutPort<Event>("placeOrderEventPort");

		state.orders = orders;

		if (!state.orders.empty()) {
			state.hasOrders = true;
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];
		}
    }

    void internalTransition(CustomerState& state) const override {
		if (state.hasOrders) {
			state.timeOfPrevOrder = state.timeOfNextOrder;
			state.indexOfNextOrder++;
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];				

			if (state.indexOfNextOrder >= state.orders.size()) {
				state.hasOrders = false;
			}
		}
    }

    void externalTransition(CustomerState& state, double e) const override {}
    
    void output(const CustomerState& state) const override {
		// Create unique order ID
		static int orderID = 0;
		orderID++;

        placeOrderEventPort->addMessage(Event(orderID));
    }

    [[nodiscard]] double timeAdvance(const CustomerState& state) const override {     
		if (state.hasOrders) {
			return state.timeOfNextOrder - state.timeOfPrevOrder;
		} else {
        	return 2147483647; 	// Maximum value for an integer, i.e., infinity.
				   				// Actually, since it's a double,
				   				// the maximum value is ~= 1.797 x 10^308,
				   				// but who's counting?
		}
    }
};

#endif // CUSTOMER_HPP
