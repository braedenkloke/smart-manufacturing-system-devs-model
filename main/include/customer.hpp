#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct CustomerState {
	std::vector<int> orders;
	bool hasOrders;
	int indexOfNextOrder;
	int timeOfNextOrder;

    explicit CustomerState(): hasOrders(false), indexOfNextOrder(0), timeOfNextOrder(0) {}
};

// Configures log format.
std::ostream& operator<<(std::ostream &out, const CustomerState& state) {
    out  << "{ hasOrders: " << state.hasOrders << " }";
    return out;
}

// Atomic DEVS model of a Customer who places orders to the system.
class Customer : public Atomic<CustomerState> {
public:
	Port<int> out;

    // Constructor.
    Customer(const std::string id, std::vector<int> orders) : Atomic<CustomerState>(id, CustomerState()) {
		out = addOutPort<int>("out");

		state.orders = orders;

		if (!state.orders.empty()) {
			state.hasOrders = true;
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];
		}
    }

    void internalTransition(CustomerState& state) const override {
		if (state.hasOrders) {
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];				
			state.indexOfNextOrder++;

			if (state.indexOfNextOrder >= state.orders.size()) {
				state.hasOrders = false;
			}
		}
    }

    void externalTransition(CustomerState& state, double e) const override {}
    
    void output(const CustomerState& state) const override {
        out->addMessage(1);
    }

    [[nodiscard]] double timeAdvance(const CustomerState& state) const override {     
		if (state.hasOrders) {
			return state.timeOfNextOrder;
		} else {
        	return 100000; // tmp placeholder to represent infinity
		}
    }
};

#endif
