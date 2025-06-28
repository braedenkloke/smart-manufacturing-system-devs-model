#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <iostream>
#include "cadmium/modeling/devs/atomic.hpp"
#include "constants.hpp"
#include "event.hpp"

using namespace cadmium;

enum CustomerStateLabel {
    WAITING,
    PLACING_ORDER,
    OUT_OF_ORDERS
};

struct CustomerState {
    CustomerStateLabel label;
    double sigma;
    std::vector<int> orders;    // Order placement times. Sorted internally in descending order 
                                // and relative to each other.

    explicit CustomerState(): label(OUT_OF_ORDERS), sigma(infinity) {}
};

#ifndef NO_LOGGING
// Formats the state log.
std::ostream& operator<<(std::ostream &out, const CustomerState& state) {
    out << "State Log: ";
    if (state.label == WAITING) {
        out << "Waiting";
    } else if (state.label == PLACING_ORDER) {
        out << "Placing Order";
    } else if (state.label == OUT_OF_ORDERS) {
        out  << "Out of Orders"; 
    }
    return out;
}
#endif

// Atomic DEVS model of a Customer who places orders to the system.
class Customer : public Atomic<CustomerState> {
public:
    Port<Event> placeOrderEventPort;

    // ARGUMENTS
    // id - Model name.
    // orders - Order placement times sorted in ascending order, i.e., the first order to place is the first
    // 			element and the last order to place is the last element. 
    Customer(const std::string id, std::vector<int> orders) : Atomic<CustomerState>(id, CustomerState()) {
        placeOrderEventPort = addOutPort<Event>("placeOrderEventPort");

        if (!orders.empty()) {
            // Sort orders in descending order, makes working with std::vector<> easier.
            while (!orders.empty()) {
                state.orders.push_back(orders.back());
                orders.pop_back();
            }
			
            // Change order placement times to be relative to one another, eliminating the need 
            // to have a 'placeholder' state variable and making time advance calculations easier.
            //
            // For example, on the left we have lists of 'absolute' order placement times sorted in 
            // descending order, and on the right we have list of 'relative' order placement times 
            // sorted in descending order. 
            //
            // [ 2, 1, 0 ] -> [ 1, 1, 0 ]
            // [ 4, 2, 1 ] -> [ 2, 1, 1 ]
            for (int i = 0; i < state.orders.size() - 1; i++) {
                state.orders[i] = state.orders[i] - state.orders[i + 1];
            }

            state.label = WAITING;
            state.sigma = state.orders.back();
            state.orders.pop_back();
        } 
    }

    void internalTransition(CustomerState& state) const override {
        if (state.label == WAITING) {
            state.label = PLACING_ORDER;
            state.sigma = 0;
        } else if (state.label == PLACING_ORDER) {
            if (!state.orders.empty()) {
                state.label = WAITING;
                state.sigma = state.orders.back();
                state.orders.pop_back();
            } else {
                state.label = OUT_OF_ORDERS;
                state.sigma = infinity;
            }
        }
    }

    void externalTransition(CustomerState& state, double e) const override {}
    
    void output(const CustomerState& state) const override {
        if (state.label == PLACING_ORDER) {
            // Create unique order ID
            static int orderID = 0;
            orderID++;

            placeOrderEventPort->addMessage(Event(orderID));
        }
    }

    [[nodiscard]] double timeAdvance(const CustomerState& state) const override {     
        return state.sigma;
    }
};

#endif // CUSTOMER_HPP
