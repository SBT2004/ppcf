#pragma once
#include "Atom.h"

class box
{
	//a dobozban atomokat tárolunk, van kiterjedése, viszont egyelőre nem fontos hogy legyen
	Atom* content_;
	struct gr {
		double* r_;
		double* gr_;

		//melyik atom melyikhez viszonyított párkorrelációs függvénye
		char* type_;
	}; 
	//készít egy indexet a megfelelő névvel rendelkező atomokról
	//a kiszámító ezen fog végigiterálni, ha pl. csak az oxigén atomok érdekelnek
	//és azok indexe 1 4 9, akkor a ciklusnak elég bejárni az 1 4 9 halmazt
	//mivel atomonként ki kell számolni a gr-t ez jelentősen gyorsabb
	size_t* index(char*name){}

	gr ppcf_single_atom(atom a, double delta_r){}

	//végigmegy az összes atomon és minden új atomtípust feljegyez
	char** get_names()

	//megállapítja milyen fajta gr-ek szükségesek,
	//hogy minden atom mindennel legyen párosítva,
	//viszont a sorrend mindegy AB BA ugyanaz, így abból csak egy kell
	char** determine_partial_gr_names(){}

};


