#include <limits>
#include "include/top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
	std::vector<int> orders;
	std::ifstream orders_input_file;
	int order;

	// Load orders
	orders_input_file.open("in/orders/default.txt");
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
