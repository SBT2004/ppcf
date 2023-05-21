#pragma once

#include <fstream>
#include <ranges>
#include <unordered_map>
#include <corecrt_math_defines.h>
#include "atom.h"
#include "dimensions.h"
#include "atom_name_pair.h"
class box_reader;

struct ppcf_values {
	double r;
	double gr;
};
class box
{
private:

	//a boxban atomokat tárolunk, emellett a kiterjedését,
	//a szimulációban eltelt időt ns-ban, és hogy ez hanyadik frame
	std::unordered_map<std::string, std::vector<atom>> atoms_;
	size_t time_;
	size_t frame_number_;
	dimensions dim_;

	

	double average_number_density_ = dim_.x * dim_.y * dim_.z * M_PI * 4 / 3;

	std::vector<double> get_distances(const atom& a, const std::string& type2);

	std::vector<ppcf_values> calculate_ppcf_single_atom(const atom& a, double dr, const std::string& type2);

	std::vector<ppcf_values> calculate_ppcf(double dr, const atom_name_pair& pair);

	static double volume(const double r) { return 4 * r * r * r * M_PI / 3; }

	static bool is_unique_pair(const std::vector<atom_name_pair>& pairs, const atom_name_pair& pair);

	//megállapítja milyen fajta ppcf_values-ek szükségesek,
	//hogy minden atomtípus minden másikkal és önmagával is legyen párosítva,
	//viszont a center sorrend mindegy, AB BA ugyanaz, így abból csak egy kell
	std::vector<atom_name_pair> ppcf_combinations();

public:
	box(const std::unordered_map<std::string, std::vector<atom>>& atoms, const dimensions& dimensions, size_t time,
		size_t frame_number) : atoms_(atoms), dim_(dimensions), time_(time), frame_number_(frame_number) { }

	std::vector<std::vector<double>> ppcf_matrix(double dr);

	friend class box_reader;
};