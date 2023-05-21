
#include <iostream>

#include "control_panel.h"


int main() {
	const control_panel cp;
	cp.initiate_analysis();
	std::cout << "Analysis complete" << std::endl;
}
