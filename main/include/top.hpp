#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "customer.hpp"
#include "mes.hpp"
#include "cell.hpp"

using namespace cadmium;

struct TopCoupled : public Coupled {

    TopCoupled(const std::string& id, std::vector<int> orders) : Coupled(id) {
        auto customer = addComponent<Customer>("customer", orders);
        auto mes = addComponent<MES>("mes");
        auto cell1 = addComponent<Cell>("cell1");

        // Couple output ports to input ports
        addCoupling(customer->placeOrderEventPort, mes->placeOrderEventPort);

        addCoupling(mes->directToLineEventPort, cell1->directToLineEventPort);
    }
};

#endif // TOP_HPP
