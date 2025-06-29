#ifndef Cell_HPP
#define Cell_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

struct CellState {
    StateLabel label;
    int sigma;
    int currentOrderID;

    explicit CellState(): label(IDLE), sigma(infinity), currentOrderID(-1) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const CellState& state) {
    out << "State Log: ";
    if (state.label == IDLE) {
        out << "Idle";
    } else if (state.label == STARTING_CELL_OPERATION) {
        out << "Starting Cell Operation";
    }
    return out;
}
#endif

// Atomic DEVS model of an assembly cell in a production line.
class Cell : public Atomic<CellState> {
public:
    Port<Event> directToLineEventPort;
    Port<Event> cellOperationStartEventPort;

    Cell(const std::string id) : Atomic<CellState>(id, CellState()) {
        directToLineEventPort = addInPort<Event>("directToLineEventPort");
        cellOperationStartEventPort = addOutPort<Event>("cellOperationStartEventPort");
    }

    void internalTransition(CellState& state) const override {
        if (state.label == STARTING_CELL_OPERATION) {
            state.label = IDLE;
            state.sigma = infinity;
            state.currentOrderID = -1;
        }
    }

    void externalTransition(CellState& state, double e) const override {
        if (state.label == IDLE) {
            if (!directToLineEventPort->empty()) {
                state.label = STARTING_CELL_OPERATION;
                state.sigma = 0;
                Event event = directToLineEventPort->getBag().back();
                state.currentOrderID = event.orderID;
            }
        }
    }
    
    void output(const CellState& state) const override {
        if (state.label == STARTING_CELL_OPERATION) {
            cellOperationStartEventPort->addMessage(Event(state.currentOrderID));
        }
    }

    [[nodiscard]] double timeAdvance(const CellState& state) const override {     
        return state.sigma;
    }
};

#endif // Cell_HPP
