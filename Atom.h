#pragma once

class atom
{
	//eltárojuk a típusát és a koordinátáit Angström-ben
	double x_;
	double y_;
	double z_;
public:
	atom(const double x_in, const double y_in, const double z_in) :x_(x_in), y_(y_in), z_(z_in) {}

	~atom() = default;

	double distance(const atom& other, double box_size) const;
};