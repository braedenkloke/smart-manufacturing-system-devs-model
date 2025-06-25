#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "customer.hpp"
#include "mes.hpp"

using namespace cadmium;

struct TopCoupled : public Coupled {

    TopCoupled(const std::string& id, std::vector<int> orders) : Coupled(id) {
        auto customer = addComponent<Customer>("customer", orders);
	auto mes = addComponent<MES>("mes");

	// Couple output ports to input ports
	addCoupling(customer->orderPlaced, mes->orderPlaced);
    }
};

#endif
