#include "atom.h"

atom::atom(const double x_in, const double y_in, const double z_in):x_(x_in),y_(y_in),z_(z_in){}

double atom::distance(const atom& other, const double box_size) const {
	double dist = sqrt((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_) + (z_ - other.z_) * (z_ - other.z_));
	const double box_diag = sqrt(3) * box_size;
	if (dist >= box_diag) dist -= box_diag;
	return dist;
}
