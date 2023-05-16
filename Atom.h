#pragma once
#include <regex>
#include <string>

class atom
{
	//elt�rojuk center t�pus�t �s center koordin�t�it Angstr�m-ben
	double x_;
	double y_;
	double z_;
public:
	atom(double x_in, double y_in, double z_in);

	~atom() = default;
	
	double distance(const atom& other, double box_size) const;
};

