#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "event_producer.hpp"

using namespace cadmium;

struct TopCoupled : public Coupled {

    TopCoupled(const std::string& id, int events[]) : Coupled(id) {
        auto eventProducer = addComponent<EventProducer>("event producer", events);
    }
};

#endif
