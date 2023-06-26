#pragma once

#include <fstream>
#include <ranges>
#include <unordered_map>
#include <corecrt_math_defines.h>
#include "atom.h"
#include "dimensions.h"
#include "atom_name_pair.h"

class box_reader;

class box
{
private:
	static constexpr double _4PI_PER3_ = M_PI * 4.0 / 3.0;

	//a boxban atomokat tárolunk, emellett a kiterjedését,
	//a szimulációban eltelt időt ns-ban, és hogy ez hanyadik frame
	std::unordered_map<std::string, std::vector<atom>> atoms_;
	size_t time_;
	size_t frame_number_;
	dimensions dim_;

	//itt kihasználtuk, hogy a doboz kocka alakú, így a sugarát bármelyik kiterjedés felének vehetjük
	const double r_max_ = dim_.x*sqrt(3)/ 2;

	//a doboz térfogata
	double box_volume_ = dim_.x * dim_.y * dim_.z;

	//kiszámolja egy atomhoz tartozó reference_atom_name nevű atomok távolságát
	//és visszaadja egy rendezett vektorként
	std::vector<size_t> get_distances(const atom& central_atom, const std::string& reference_atom_name, const double dr);

	//kiszámolja egy atomhoz tartozó reference_atom_name nevű atomok relatív gyakoriságát
	std::vector<double> calculate_gr_single_atom(const atom& central_atom, double dr, const std::string& reference_atom_name);

	//kiszámolja a boxban lévő összes, pairben tárolt nevű atom relatív gyakoriságát egymáshoz képest
	std::vector<double> calculate_gr(double dr, const atom_name_pair& pair);

	//egy adott sugarú gömb térfogata
	static double volume(const double r) { return r * r * r * _4PI_PER3_; }

	//Meghatározza, hogy milyen atomok-referencia párok között kell ppcf számítást végezni,
	//A sorrend mindegy AB BA ugyanaz, így az csak 1 pár
	std::vector<atom_name_pair> ppcf_combinations();

	//megnézi, hogy egy adott atomnév-pár már szerepel-e a vektorban, sorrendtől függetlenül
	static bool is_unique_pair(const std::vector<atom_name_pair>& pairs, const atom_name_pair& pair);

	//creates an artihmetic sequence with a start and endpoint and dq difference
	static std::vector<double> linspace(double start, size_t length, double increment);

public:

	box(std::unordered_map<std::string, std::vector<atom>> atoms, const dimensions& dimensions, const size_t time,
		const size_t frame_number) : atoms_(std::move(atoms)), time_(time), frame_number_(frame_number), dim_(dimensions) { }

	//Meghívja a ppcf értékeket kiszámító függvényeket minden érvényes párra,
	//majd ezeket összerakja egy mátrixba,
	//Az első oszlopba r értékeket generálja.
	std::vector<std::vector<double>> ppcf_matrix(double dr);

	friend class box_reader;
};