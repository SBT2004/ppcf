#include <iostream>
#include <chrono>
#include "control_panel.h"


int main() {
	const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	constexpr control_panel cp;
	cp.initiate_analysis();
	std::cout << "Analysis complete" << std::endl;

	const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;
}
