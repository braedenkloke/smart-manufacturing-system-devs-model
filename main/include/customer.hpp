#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct CustomerState {
	bool hasOrders;
	int* orders;
	int totalOrders;
	int indexOfNextOrder;
	int timeOfNextOrder;
    //you can have as many state variables as you want/ need

    explicit CustomerState(): hasOrders(false), totalOrders(0), indexOfNextOrder(0), timeOfNextOrder(0) {}
};

// Configures log format.
std::ostream& operator<<(std::ostream &out, const CustomerState& state) {
    out  << "{ timeOfNextOrder: " << state.timeOfNextOrder << ", totalOrders: " << state.totalOrders << " }";
    return out;
}

// Atomic DEVS model of a Customer who places orders to the system.
class Customer : public Atomic<CustomerState> {
    public:

    //Declare your ports here

    // Constructor.
    Customer(const std::string id, int* orders) : Atomic<CustomerState>(id, CustomerState()) {
        //Initialize ports here.

		state.orders = orders;
		state.totalOrders = 4; // stubbed

		if (state.totalOrders != 0) {
			state.hasOrders = true;
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];
		}
    }

    void internalTransition(CustomerState& state) const override {
		if (state.hasOrders && state.indexOfNextOrder < state.totalOrders) {
			state.timeOfNextOrder = state.orders[state.indexOfNextOrder];
			state.indexOfNextOrder++;
		} else {
			state.hasOrders = false;
		}
    }

    void externalTransition(CustomerState& state, double e) const override {}
    
    void output(const CustomerState& state) const override {
        //your output function goes here
    }

    [[nodiscard]] double timeAdvance(const CustomerState& state) const override {     
		if (state.hasOrders) {
			return state.timeOfNextOrder;
		} else {
        	return 10000000000; // tmp placeholder to represent infinity
		}
    }
};

#endif
