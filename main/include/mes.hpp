#ifndef MES_HPP
#define MES_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "events/place_order_event.hpp"

using namespace cadmium;

struct MESState {
	bool idle;
	bool initiatingOrder;

    explicit MESState(): idle(true), initiatingOrder(false) {}
};

// Configures log format.
std::ostream& operator<<(std::ostream &out, const MESState& state) {
    out  << "{ idle: " << state.idle << ", initiatingOrder: " << state.initiatingOrder << " }";
    return out;
}

// Atomic DEVS model of a Manufacturing Execution System (MES).
class MES : public Atomic<MESState> {
public:
	Port<PlaceOrderEvent> placeOrderEventPort;
	Port<std::string> orderInitiated;

    // Constructor.
    MES(const std::string id) : Atomic<MESState>(id, MESState()) {
		placeOrderEventPort = addInPort<PlaceOrderEvent>("placeOrderEventPort");
		orderInitiated = addOutPort<std::string>("orderInitiated");
    }

    void internalTransition(MESState& state) const override {
		if (state.initiatingOrder) {
			state.initiatingOrder = false;
			state.idle = true;
		}
    }

    void externalTransition(MESState& state, double e) const override {
		if (state.idle && !placeOrderEventPort->empty()) {
			state.idle = false;
			state.initiatingOrder = true;
		}
	}
    
    void output(const MESState& state) const override {
        orderInitiated->addMessage("Order Initiated");
    }

    [[nodiscard]] double timeAdvance(const MESState& state) const override {     
		if (state.idle) {
        	return 2147483647; 	// Maximum value for an integer, i.e., infinity.
				   				// Actually, since it's a double,
				   				// the maximum value is ~= 1.797 x 10^308,
				   				// but who's counting?
		} else {
			return 0;
		}
    }
};

#endif // MES_HPP
