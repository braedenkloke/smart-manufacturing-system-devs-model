#include <limits>
#include "include/top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
	int order_confirmation_times[] = {0, 1, 2, 4}; // seconds

	auto model = std::make_shared<top_coupled>("top", order_confirmation_times);
	auto rootCoordinator = cadmium::RootCoordinator(model);

	rootCoordinator.setLogger<STDOUTLogger>(";");
	rootCoordinator.start();
	rootCoordinator.simulate(22.0);
	rootCoordinator.stop();	
}
