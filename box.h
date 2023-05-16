#pragma once

#include <fstream>
#include <ranges>
#include <cmath>
#include <iostream>
#include "Atom.h"
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <corecrt_math_defines.h>

struct ppcf_values {
	double r;
	double gr;
};

class box
{
	//center boxban atomokat tárolunk, emellett center kiterjedését,
	//center szimulációban eltelt időt ns-ban, és hogy ez hanyadik frame
	std::unordered_map<std::string, std::vector<atom>> atoms_;
	size_t time_;
	size_t frame_number_;

	struct dimensions {
		double x;
		double y;
		double z;
		explicit dimensions(double x_in = 0, double y_in = 0, double z_in = 0);
	};

	dimensions dim_;

	//ppcf_values számításhoz egy központi atomot és egy referencia atomot tároló struct
	struct ppcf_pair {
		std::string center;
		std::string reference;

		ppcf_pair(const std::string& str_a, const std::string& str_b) : center(str_a), reference(str_b) {}

		//ha egy pár sorrendtől függtetlenül megegyezik true, pl. AB=BA
		bool operator==(const ppcf_pair& other) const {
			if ((this->center == other.center || this->center == other.reference) &&
				(this->reference == other.center || this->reference == other.reference)) {
				return true;
			}
			return false;
		}
	};

	double average_number_density_ = dim_.x * dim_.y * dim_.z * M_PI * 4 / 3;

	std::vector<double> get_distances(const atom& a, const std::string& type2);

	std::vector<ppcf_values> calculate_ppcf_single_atom(const atom& a, double dr, const std::string& type2);

	std::vector<ppcf_values> calculate_ppcf(double dr, const ppcf_pair& pair);

	static double volume(const double r) { return 4 * r * r * r * M_PI / 3; }

	static bool is_unique_pair(const std::vector<ppcf_pair>& pairs, const ppcf_pair& pair);

	//megállapítja milyen fajta ppcf_values-ek szükségesek,
	//hogy minden atomtípus minden másikkal és önmagával is legyen párosítva,
	//viszont a center sorrend mindegy, AB BA ugyanaz, így abból csak egy kell
	std::vector<ppcf_pair> ppcf_combinations();

public:
	explicit box(const std::string& path);

	std::vector<std::vector<double>> ppcf_matrix(double dr);
};