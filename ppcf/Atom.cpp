#include "Atom.h"

atom::atom(const std::smatch& matches) {
	//a 0. találatot nem itt tároljuk
	x_ = std::stod(matches[1]);
	y_ = std::stod(matches[2]);
	z_ = std::stod(matches[3]);
}

double atom::distance(const atom& other) const {
	return sqrt((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_) + (z_ - other.z_) * (z_ - other.z_));
}
