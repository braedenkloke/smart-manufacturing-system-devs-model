#ifndef EVENT_PRODUCER_HPP
#define EVENT_PRODUCER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct EventProducerState {
	bool producingEvents;
	int* events;
	int totalNumEvents;
	int indexOfNextEvent;
	int timeOfNextEvent;
    //you can have as many state variables as you want/ need

    explicit EventProducerState(): producingEvents(false), totalNumEvents(0), indexOfNextEvent(0), timeOfNextEvent(0) {}
};

// Configures log format.
std::ostream& operator<<(std::ostream &out, const EventProducerState& state) {
    out  << "{ timeOfNextEvent: " << state.timeOfNextEvent << ", totalNumEvents: " << state.totalNumEvents << " }";
    return out;
}

class EventProducer : public Atomic<EventProducerState> {
    public:

    //Declare your ports here

    // Constructor.
    EventProducer(const std::string id, int* events) : Atomic<EventProducerState>(id, EventProducerState()) {
        //Initialize ports here.

		state.events = events;
		//state.totalNumEvents = sizeof(state.events) / sizeof(state.events[0]);
		state.totalNumEvents = 4;

		if (state.totalNumEvents != 0) {
			state.producingEvents = true;
			state.timeOfNextEvent = state.events[state.indexOfNextEvent];
		}
    }

    void internalTransition(EventProducerState& state) const override {
		if (state.producingEvents && state.indexOfNextEvent < state.totalNumEvents) {
			state.timeOfNextEvent = state.events[state.indexOfNextEvent];
			state.indexOfNextEvent++;
		} else {
			state.producingEvents = false;
		}
    }

    void externalTransition(EventProducerState& state, double e) const override {}
    
    void output(const EventProducerState& state) const override {
        //your output function goes here
    }

    [[nodiscard]] double timeAdvance(const EventProducerState& state) const override {     
		if (state.producingEvents) {
			return state.timeOfNextEvent;
		} else {
        	return 10000000000; // tmp placeholder to represent infinity
		}
    }
};

#endif
