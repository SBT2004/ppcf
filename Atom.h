#pragma once
#include <regex>
#include <string>

class atom
{
	//eltárojuk a típusát és a koordinátáit Angström-ben
	double x_;
	double y_;
	double z_;
public:
	//a ctor beolvas egy sort a fájlból, értelmezi és inicializálja az értékeket
	//defaultra nincs szükség, hiszen az atomtípust tudni kell
	explicit atom(const std::smatch& matches);

	~atom() = default;

	double distance(const atom& other) const;
	
};

