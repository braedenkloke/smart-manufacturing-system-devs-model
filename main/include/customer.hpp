#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

struct CustomerState {
	std::vector<int> orders; // Internally sorted in descending order
	int timeOfNextOrder; // -1 if there are no more orders

    explicit CustomerState(): timeOfNextOrder(-1) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const CustomerState& state) {
    out  << "{ timeOfNextOrder: " << state.timeOfNextOrder << " }";
    return out;
}
#endif

// Atomic DEVS model of a Customer who places orders to the system.
class Customer : public Atomic<CustomerState> {
public:
	Port<Event> placeOrderEventPort;

	// ARGUMENTS
    // id - Model name.
	// orders - Orders to place sorted in ascending order, i.e., the first order to place is the first
    // 			element and the last order to place is the last element. 
    Customer(const std::string id, std::vector<int> orders) : Atomic<CustomerState>(id, CustomerState()) {
		placeOrderEventPort = addOutPort<Event>("placeOrderEventPort");

		// Sort orders in descending order, makes working with std::vector<> easier.
		while (!orders.empty()) {
			state.orders.push_back(orders.back());
			orders.pop_back();
		}

		if (!state.orders.empty()) {
			state.timeOfNextOrder = state.orders.back();
			state.orders.pop_back();
		} else {
			state.timeOfNextOrder = -1;
		}
    }

    void internalTransition(CustomerState& state) const override {
		if (!state.orders.empty()) {
			state.timeOfNextOrder = state.orders.back() - state.timeOfNextOrder;
			state.orders.pop_back();
		} else {
			state.timeOfNextOrder = -1;
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
		if (state.timeOfNextOrder >= 0) {
			return state.timeOfNextOrder;
		} else {
			return infinity;
		}
    }
};

#endif // CUSTOMER_HPP
