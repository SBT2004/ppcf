#pragma once
#include <regex>
#include <string>

class atom
{
	//elt�rojuk a t�pus�t �s a koordin�t�it Angstr�m-ben
	double x_;
	double y_;
	double z_;
public:
	//a ctor beolvas egy sort a f�jlb�l, �rtelmezi �s inicializ�lja az �rt�keket
	//defaultra nincs sz�ks�g, hiszen az atomt�pust tudni kell
	explicit atom(const std::smatch& matches);

	~atom() = default;

	double distance(const atom& other) const;
	
};

