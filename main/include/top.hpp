#ifndef SAMPLE_TOP_HPP
#define SAMPLE_TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "event_producer.hpp"

using namespace cadmium;

struct top_coupled : public Coupled {

    /**
     * Constructor function for the smart manufacturing system model.
     * @param id ID of the smart manufacturing system model.
     */
    top_coupled(const std::string& id) : Coupled(id) {
        auto event_producer_1 = addComponent<event_producer>("event producer");
    }
};

#endif
