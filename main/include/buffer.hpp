#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

struct BufferState {
    Phase phase;
    int sigma;
    std::vector<int> buffer;
    int capacity;
    int response;

    explicit BufferState(): phase(PASSIVE), sigma(infinity) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const BufferState& state) {
    out << "State Log: ";
    if (state.phase == PASSIVE) {
        out << "Passive";
    } else if (state.phase == ADDING_TO_BUFFER) {
        out << "Adding to Buffer";
    } else if (state.phase == REMOVING_FROM_BUFFER) {
        out << "Removing from Buffer";
    }
    return out;
}
#endif

// Passive atomic DEVS model of a buffer that holds orders.
// This model assumes there is at most one production line component adding to the buffer
// and at most one production line component removing from the buffer, 
// e.g., one MES adding orders and one Cell removing orders.
//
// INPUT PORTS
// int add - Add OrderID to buffer.
// int remove - Remove next OrderID from buffer.  
//
// OUTPUT PORTS
// int addResponse - Returns added OrderID or -1 if buffer is full.
// int removeResponse - Returns removed OrderID or -1 if buffer is empty.
class Buffer : public Atomic<BufferState> {
public:
    Port<int> add;
    Port<int> addResponse;
    Port<int> remove;
    Port<int> removeResponse;

    // ARGUMENTS
    // id - Model name.
    // capacity - Maximum number of elements buffer can contain.
    Buffer(const std::string id, int capacity) : Atomic<BufferState>(id, BufferState()) {
        add = addInPort<int>("add");
        addResponse = addOutPort<int>("addResponse");
        remove = addInPort<int>("remove");
        removeResponse = addOutPort<int>("removeResponse");

        state.capacity = capacity;
    }

    void internalTransition(BufferState& state) const override {
        if (state.phase == ADDING_TO_BUFFER || state.phase == REMOVING_FROM_BUFFER) {
            state.phase = PASSIVE;
            state.sigma = infinity;
        }
    }

    void externalTransition(BufferState& state, double e) const override {
        // The following code implements the buffer as a FIFO queue.
        if (state.phase == PASSIVE) {
            if (!add->empty()) {
                state.phase = ADDING_TO_BUFFER;
                state.sigma = 0;
                if (state.buffer.size() < state.capacity) {
                    // Add element to end of queue.
                    state.buffer.push_back(add->getBag().back());
                    state.response = state.buffer.back();
                } else {
                    // Buffer is full.
                    state.response = -1;
                }
            } else if (!remove->empty()) {
                state.phase = REMOVING_FROM_BUFFER;
                state.sigma = 0;
                if (!state.buffer.empty()) {
                    // Remove element from front of queue.
                    state.response = state.buffer[0];
                    for (int i; i < state.buffer.size() - 1; i++) {
                        state.buffer[i] = state.buffer[i + 1];
                    }
                    state.buffer.pop_back();
                } else {
                    // Buffer is empty.
                    state.response = -1;
                }
            }
        }
    }
    
    void output(const BufferState& state) const override {
        if (state.phase == ADDING_TO_BUFFER) {
            addResponse->addMessage(state.response);
        } else if (state.phase == REMOVING_FROM_BUFFER) {
            removeResponse->addMessage(state.response);
        }
    }

    [[nodiscard]] double timeAdvance(const BufferState& state) const override {     
        return state.sigma;
    }
};

#endif // BUFFER_HPP
