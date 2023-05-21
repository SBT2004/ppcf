#pragma once

class atom
{
	//elt�rojuk a t�pus�t �s a koordin�t�it Angstr�m-ben
	double x_;
	double y_;
	double z_;
public:
	atom(const double x_in, const double y_in, const double z_in) :x_(x_in), y_(y_in), z_(z_in) {}

	~atom() = default;

	double distance(const atom& other, double box_size) const;
};