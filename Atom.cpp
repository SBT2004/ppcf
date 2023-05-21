#include "atom.h"
#include <cmath>

double atom::distance(const atom& other, const double box_size) const {
	double dist = sqrt((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_) + (z_ - other.z_) * (z_ - other.z_));
	if (const double box_diag = sqrt(3) * box_size; dist >= box_diag) {
		dist -= box_diag;
	}
	return dist;
}