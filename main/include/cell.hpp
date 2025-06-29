#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

// Prefixed with class name to avoid namespace collisions 
// from models with similarly named phases.
enum CellPhase {
    // Sorted alphabetically.
    CELL_ASSEMBLING,
    CELL_CHECKING_BUFFER,
    CELL_STARTING_ASSEMBLY,
    CELL_WAITING_FOR_BUFFER_RESPONSE
};

struct CellState {
    CellPhase phase;
    int sigma;
    int currentOrderID;
    double bufferPollingInterval; // seconds

    explicit CellState(): phase(CELL_CHECKING_BUFFER), sigma(0), 
                          currentOrderID(-1), bufferPollingInterval(1) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const CellState& state) {
    out << "State Log: ";
    if (state.phase == CELL_ASSEMBLING) {
        out << "Assembling";
    } else if (state.phase == CELL_CHECKING_BUFFER) {
        out << "Checking Buffer";
    } else if (state.phase == CELL_STARTING_ASSEMBLY) {
        out << "Starting Assembly";
    } else if (state.phase == CELL_WAITING_FOR_BUFFER_RESPONSE) {
        out << "Waiting for Buffer Response";
    }
    return out;
}
#endif

// Atomic DEVS model of an assembly cell in a production line.
// The cell periodically polls its buffer to see if new orders have arrived.
class Cell : public Atomic<CellState> {
public:
    Port<int> bufferRemove;
    Port<int> bufferRemoveResponse;
    Port<Event> cellOperationStartEventPort;
    Port<Event> cellOperationEndEventPort;

    Cell(const std::string id) : Atomic<CellState>(id, CellState()) {
        bufferRemove = addOutPort<int>("bufferRemove");
        bufferRemoveResponse = addInPort<int>("bufferRemoveResponse");
        cellOperationStartEventPort = addOutPort<Event>("cellOperationStartEventPort");
        cellOperationEndEventPort = addOutPort<Event>("cellOperationEndEventPort");
    }

    void internalTransition(CellState& state) const override {
        if (state.phase == CELL_CHECKING_BUFFER) {
            state.phase = CELL_WAITING_FOR_BUFFER_RESPONSE;
            state.sigma = infinity;
        } else if (state.phase == CELL_STARTING_ASSEMBLY) {
            state.phase = CELL_ASSEMBLING;
            state.sigma = 0; // Time to assemble order.
        } else if (state.phase == CELL_ASSEMBLING) { 
            state.phase = CELL_CHECKING_BUFFER;
            state.sigma = state.bufferPollingInterval;
        }
    }

    void externalTransition(CellState& state, double e) const override {
        if (state.phase == CELL_WAITING_FOR_BUFFER_RESPONSE) {
            if (!bufferRemoveResponse->empty()) {
                state.currentOrderID = bufferRemoveResponse->getBag().back();
                if (state.currentOrderID >= 0) {
                    state.phase = CELL_STARTING_ASSEMBLY;
                    state.sigma = 0;
                } else {
                    state.phase = CELL_CHECKING_BUFFER;
                    state.sigma = state.bufferPollingInterval;
                }
            }
        }
    }
    
    void output(const CellState& state) const override {
        if (state.phase == CELL_CHECKING_BUFFER) {
            bufferRemove->addMessage(0);
        } else if (state.phase == CELL_STARTING_ASSEMBLY) {
            cellOperationStartEventPort->addMessage(Event(state.currentOrderID));
        } else if (state.phase == CELL_ASSEMBLING) {
            cellOperationEndEventPort->addMessage(Event(state.currentOrderID));
        }
    }

    [[nodiscard]] double timeAdvance(const CellState& state) const override {     
        return state.sigma;
    }
};

#endif // CELL_HPP
