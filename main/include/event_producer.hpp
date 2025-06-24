#ifndef EVENT_PRODUCER_HPP
#define EVENT_PRODUCER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct event_producerState {
    double sigma;
    //you can have as many state variables as you want/ need

    explicit event_producerState(): sigma(1){
    }
};

// Configures log format.
std::ostream& operator<<(std::ostream &out, const event_producerState& state) {
    out  << "{" << state.sigma << "}";
    return out;
}

class event_producer : public Atomic<event_producerState> {
    public:

    //Declare your ports here

    // Constructor.
    event_producer(const std::string id, int* order_confirmation_times) : Atomic<event_producerState>(id, event_producerState()) {
        //Initialize ports here.
    }

    void internalTransition(event_producerState& state) const override {
        //your internal transition function goes here
        state.sigma += 3;
    }

    void externalTransition(event_producerState& state, double e) const override {
        //your external transition function hoes here
    }
    
    
    void output(const event_producerState& state) const override {
        //your output function goes here
    }

    [[nodiscard]] double timeAdvance(const event_producerState& state) const override {     
            return state.sigma;
    }
};

#endif
