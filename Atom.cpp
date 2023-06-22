#include "atom.h"
#include <cmath>

//Megat�rozza a k�t atom t�vols�g�t, periodikus hat�rfelt�telekkel sz�mol,
//azaz, k�t atom t�vols�ga a legkisebb t�vols�g a k�zponti atom �s a referencia t�k�rk�pei k�z�tt.
//Praktikusan ha b�rmelyik ir�nyban a k�t atom t�vols�ga nagyobb mint a box_size/2,
//akkor azt a kooordin�t�t box_size-b�l kell kivonni.
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