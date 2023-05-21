#pragma once

#include <fstream>
#include <ranges>
#include <unordered_map>
#include <corecrt_math_defines.h>
#include "atom.h"
#include "dimensions.h"
#include "atom_name_pair.h"
class box_reader;

struct ppcf_value_pair {
	double r;
	double gr;
};

class box
{
private:
	//TODO ezt szebben
	static constexpr double _43PI_ = M_PI * 4 / 3;
	//a boxban atomokat tárolunk, emellett a kiterjedését,
	//a szimulációban eltelt időt ns-ban, és hogy ez hanyadik frame
	std::unordered_map<std::string, std::vector<atom>> atoms_;
	size_t time_;
	size_t frame_number_;
	dimensions dim_;

	//itt kihasználtuk, hogy a doboz kocka alakú
	const double r_max_ = dim_.x / 2;
	double box_volume_ = dim_.x * dim_.y * dim_.z * M_PI * 4 / 3;

	//kiszámolja egy atomhoz tartozó type2 nevű atomok távolságát
	//és visszaadja egy rendezett vektorként
	std::vector<double> get_distances(const atom& a, const std::string& type2);

	//kiszámolja egy atomhoz tartozó type2 nevű atomok relatív gyakoriságát
	std::vector<double> calculate_gr_single_atom(const atom& a, double dr, const std::string& type2);

	//kiszámolja a boxban lévő összes, pair-ben tárolt nevű atom relatív gyakoriságát egymáshoz képest
	std::vector<double> calculate_gr(double dr, const atom_name_pair& pair);

	static double volume(const double r) { return 4 * r * r * r * M_PI / 3; }

	//megállapítja milyen fajta ppcf_value_pair-ek szükségesek,
	//hogy minden atomtípus minden másikkal és önmagával is legyen párosítva,
	//viszont a center sorrend mindegy, AB BA ugyanaz, így abból csak egy kell
	std::vector<atom_name_pair> ppcf_combinations();

	static bool is_unique_pair(const std::vector<atom_name_pair>& pairs, const atom_name_pair& pair);

	//számtani sorozatot hoz létre, start és end között num_points darab pontban
	static std::vector<double> linspace(double start, double end, int num_points);

public:
	box(std::unordered_map<std::string, std::vector<atom>> atoms, const dimensions& dimensions, const size_t time,
	    const size_t frame_number) : atoms_(std::move(atoms)), time_(time), frame_number_(frame_number), dim_(dimensions) { }

	std::vector<std::vector<double>> ppcf_matrix(double dr);

	friend class box_reader;
};