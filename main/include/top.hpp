#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "customer.hpp"
#include "mes.hpp"
#include "buffer.hpp"
#include "cell.hpp"

using namespace cadmium;

struct TopCoupled : public Coupled {

    TopCoupled(const std::string& id, std::vector<int> orders) : Coupled(id) {
        auto customer = addComponent<Customer>("customer", orders);
        auto mes = addComponent<MES>("mes");
        auto cell1Buffer = addComponent<Buffer>("cell1Buffer", infinity);
        auto cell1 = addComponent<Cell>("cell1");

        // Couple output ports to input ports
        addCoupling(customer->placeOrderEventPort, mes->placeOrderEventPort);

        addCoupling(mes->addLine1Buffer, cell1Buffer->add);

        addCoupling(cell1->bufferRemove, cell1Buffer->remove);
        addCoupling(cell1Buffer->removeResponse, cell1->bufferRemoveResponse);
    }
};

#endif // TOP_HPP
