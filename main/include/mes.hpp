#ifndef MES_HPP
#define MES_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

struct MESState {
	int idOfCurrentOrder; // -1 when MES is idle, i.e., not processing an order
	bool initiatingNewOrder;

    explicit MESState(): idOfCurrentOrder(-1), initiatingNewOrder(false) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const MESState& state) {
    out  << "{ idOfCurrentOrder: " << state.idOfCurrentOrder << ", initiatingNewOrder: " << state.initiatingNewOrder << " }";
    return out;
}
#endif

// Atomic DEVS model of a Manufacturing Execution System (MES).
class MES : public Atomic<MESState> {
public:
	Port<Event> newOrderEventPort;
	Port<Event> placeOrderEventPort;

    MES(const std::string id) : Atomic<MESState>(id, MESState()) {
		placeOrderEventPort = addInPort<Event>("placeOrderEventPort");
		newOrderEventPort = addOutPort<Event>("newOrderEventPort");
    }

    void internalTransition(MESState& state) const override {
		if (state.idOfCurrentOrder >= 0) {
			if (state.initiatingNewOrder) {
				state.initiatingNewOrder = false;
				state.idOfCurrentOrder = -1;
			}	
		}
    }

    void externalTransition(MESState& state, double e) const override {
		// Check if MES is idle
		if (state.idOfCurrentOrder < 0) {
			if (!placeOrderEventPort->empty()) {
				state.initiatingNewOrder = true;
				Event event = placeOrderEventPort->getBag().back();
				state.idOfCurrentOrder = event.orderID;
			}
		}
	}
    
    void output(const MESState& state) const override {
		if (state.idOfCurrentOrder >= 0) {
			if (state.initiatingNewOrder) {
        		newOrderEventPort->addMessage(Event(state.idOfCurrentOrder));
			}
		}
    }

    [[nodiscard]] double timeAdvance(const MESState& state) const override {     
		if (state.idOfCurrentOrder >= 0) {
			return 0;
		} else {
			return infinity;
		}
    }
};

#endif // MES_HPP
