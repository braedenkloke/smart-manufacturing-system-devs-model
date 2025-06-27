#ifndef MES_HPP
#define MES_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "event.hpp"

using namespace cadmium;

struct MESState {
	bool idle;
	bool initiatingOrder;
	int idOfCurrentOrder;

    explicit MESState(): idle(true), initiatingOrder(false), idOfCurrentOrder(0) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const MESState& state) {
    out  << "{ idle: " << state.idle << ", initiatingOrder: " << state.initiatingOrder << " }";
    return out;
}
#endif

// Atomic DEVS model of a Manufacturing Execution System (MES).
class MES : public Atomic<MESState> {
public:
	Port<Event> newOrderEventPort;
	Port<Event> placeOrderEventPort;

    // Constructor.
    MES(const std::string id) : Atomic<MESState>(id, MESState()) {
		placeOrderEventPort = addInPort<Event>("placeOrderEventPort");
		newOrderEventPort = addOutPort<Event>("newOrderEventPort");
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
			Event event = placeOrderEventPort->getBag().back();
			state.idOfCurrentOrder = event.orderID;
		}
	}
    
    void output(const MESState& state) const override {
        newOrderEventPort->addMessage(Event(state.idOfCurrentOrder));
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
