#include "problem.h"

constexpr const std::size_t ITERATION_AMOUNT{ 0 };

int main(int argc, char** argv)
{
	ProcessSimulator process_simulator(0.25, 0.25, 0.75, 0.75, ITERATION_AMOUNT);

	process_simulator.simulateProcess();

	return EXIT_SUCCESS;
}