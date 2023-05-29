#include "atom.h"
#include <cmath>

//Megatározza a két atom távolságát, periodikus határfeltételekkel számol,
//azaz, két atom távolsága a legkisebb távolság a központi atom és a referencia tükörképei között.
//Praktikusan ha bármelyik irányban a két atom távolsága nagyobb mint a box_size/2,
//akkor azt a kooordinátát box_size-ból kell kivonni.
double atom::distance(const atom& other, const double box_size) const {
	double x, y, z;

	if (x = abs(x_ - other.x_); x >= box_size / 2) {
		x = box_size - abs(x_ - other.x_);
	}
	if (y = abs(y_ - other.y_); y >= box_size / 2) {
		y = box_size - abs(y_ - other.y_);
	}
	if (z = abs(z_ - other.z_); z >= box_size / 2) {
		z = box_size - abs(z_ - other.z_);
	}

	return sqrt(x * x + y * y + z * z);
}