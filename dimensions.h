#pragma once

struct dimensions {
	double x;
	double y;
	double z;

	dimensions() = default;

	dimensions(const double x_in, const double y_in, const double z_in) : x(x_in), y(y_in), z(z_in) {}
};


