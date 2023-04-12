#pragma once
#include <fstream>

class Atom
{
	//eltárojuk a típusát és a koordinátáit Angström-ben
	double x_;
	double y_;
	double z_;
	char* type;

	//ctor
	//defaultra nincs szükség, hiszen az atomtípust tudni kell
	//a ctor beolvas egy sort a fájlból, értelmezi és inicializálja az értékeket
	//extra feature: rename ha van gyári név,
	//akkor átnevezi az atomot megadott névre,
	//ezt a metódust hívja majd a ctor
	Atom(std::ifstream file_state_pointer, char* name);
	void rename (char* name){}
	double distance (const Atom other) const {
		return sqrt((x_-other.x_)*(x_-other.x_)+(y_-other.y_)*(y_-other.y_)+(z_-other.z_)*(z_-other.z_));
	}

};

