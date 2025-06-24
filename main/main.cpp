#include <limits>
#include "include/top.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "cadmium/simulation/logger/csv.hpp"

using namespace cadmium;

int main() {
	auto model = std::make_shared<top_coupled>("top");
	auto rootCoordinator = cadmium::RootCoordinator(model);

	rootCoordinator.setLogger<STDOUTLogger>(";");
	rootCoordinator.start();
	rootCoordinator.simulate(22.0);
	rootCoordinator.stop();	
}
