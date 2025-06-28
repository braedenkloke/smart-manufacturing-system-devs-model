#include <limits>
#include "include/top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main(int argc, char* argv[]) {
    std::vector<int> orders;
    std::ifstream orders_input_file;
    std::string orders_input_file_path;

    if (argc == 1) {
        orders_input_file_path = "in/orders/default.txt";
    } else if (argc == 2) {
        orders_input_file_path = argv[1];
    } else {
        std::cout << "Invalid number of arguments ... aborting gracefully.";
        return 1;
	}

    // Load orders
    orders_input_file.open(orders_input_file_path);

    int order;
    while (orders_input_file >> order) {
        orders.push_back(order);
    }

    auto model = std::make_shared<TopCoupled>("top", orders);
    auto rootCoordinator = cadmium::RootCoordinator(model);

    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(22.0);
    rootCoordinator.stop();	
}
