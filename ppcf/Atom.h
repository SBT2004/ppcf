#pragma once
#include <fstream>

class Atom
{
	//elt�rojuk a t�pus�t �s a koordin�t�it Angstr�m-ben
	double x_;
	double y_;
	double z_;
	char* type;

	//ctor
	//defaultra nincs sz�ks�g, hiszen az atomt�pust tudni kell
	//a ctor beolvas egy sort a f�jlb�l, �rtelmezi �s inicializ�lja az �rt�keket
	//extra feature: rename ha van gy�ri n�v,
	//akkor �tnevezi az atomot megadott n�vre,
	//ezt a met�dust h�vja majd a ctor
	Atom(std::ifstream file_state_pointer, char* name);
	void rename (char* name){}
	double distance (const Atom other) const {
		return sqrt((x_-other.x_)*(x_-other.x_)+(y_-other.y_)*(y_-other.y_)+(z_-other.z_)*(z_-other.z_));
	}

};

