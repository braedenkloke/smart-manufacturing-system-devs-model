#include <limits>
#include "include/top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
	int events[] = {0, 1, 2, 4}; // seconds

	auto model = std::make_shared<TopCoupled>("top", events);
	auto rootCoordinator = cadmium::RootCoordinator(model);

	rootCoordinator.setLogger<STDOUTLogger>(";");
	rootCoordinator.start();
	rootCoordinator.simulate(22.0);
	rootCoordinator.stop();	
}
