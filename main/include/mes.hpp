#ifndef MES_HPP
#define MES_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

struct MESState {
    Phase phase;
    int sigma;
    int currentOrderID;

    explicit MESState(): phase(IDLE), sigma(infinity), currentOrderID(-1) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const MESState& state) {
    out << "State Log: ";
    if (state.phase == IDLE) {
        out << "Idle";
    } else if (state.phase == INITIATING_NEW_ORDER) {
        out << "Initiating New Order";
    } else if (state.phase == DIRECTING_ORDER_TO_LINE) {
        out << "Directing Order to Line";
    }
    return out;
}
#endif

// Atomic DEVS model of a Manufacturing Execution System (MES).
class MES : public Atomic<MESState> {
public:
    Port<Event> newOrderEventPort;
    Port<Event> placeOrderEventPort;
    Port<Event> directToLine1EventPort;
    Port<int> addLine1Buffer;

    MES(const std::string id) : Atomic<MESState>(id, MESState()) {
        placeOrderEventPort = addInPort<Event>("placeOrderEventPort");
        newOrderEventPort = addOutPort<Event>("newOrderEventPort");
        directToLine1EventPort = addOutPort<Event>("directToLine1EventPort");
        addLine1Buffer = addOutPort<int>("addLine1Buffer");
    }

    void internalTransition(MESState& state) const override {
        if (state.phase == INITIATING_NEW_ORDER) {
            state.phase = DIRECTING_ORDER_TO_LINE;
            state.sigma = 0;
        } else if (state.phase == DIRECTING_ORDER_TO_LINE) {
            state.phase = IDLE;
            state.sigma = infinity;
            state.currentOrderID = -1;
        }
    }

    void externalTransition(MESState& state, double e) const override {
        if (state.phase == IDLE) {
            if (!placeOrderEventPort->empty()) {
                state.phase = INITIATING_NEW_ORDER;
                state.sigma = 0;
                Event event = placeOrderEventPort->getBag().back();
                state.currentOrderID = event.orderID;
            }
        }
    }
    
    void output(const MESState& state) const override {
        if (state.phase == INITIATING_NEW_ORDER) {
            newOrderEventPort->addMessage(Event(state.currentOrderID));
        } else if (state.phase == DIRECTING_ORDER_TO_LINE) {
            directToLine1EventPort->addMessage(Event(state.currentOrderID));
            addLine1Buffer->addMessage(state.currentOrderID); // No need to check response, unlimited buffer size.
        }
    }

    [[nodiscard]] double timeAdvance(const MESState& state) const override {     
        return state.sigma;
    }
};

#endif // MES_HPP
