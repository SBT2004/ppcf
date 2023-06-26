#include "box.h"
#include <algorithm>

std::vector<size_t> box::get_distances(const atom& central_atom,
	const std::string& reference_atom_name,const double dr) {
	const size_t number_of_bins = static_cast<size_t>(r_max_ / dr) + 1;

	//the number of atoms in a bin will be stored in this vector
	std::vector<size_t> distances(number_of_bins);

	//the atom can't recognise itself, it must be subtracted from the first bin
	distances[0]=-1;
	//sort the atom distances into bins, only the number of atoms in the bins are stored
	for (const auto& ref : atoms_[reference_atom_name]) {
		const double distance = central_atom.distance(ref, dim_.x);
		const int i = static_cast<int>(distance / dr);
		distances[i]++;
	}
	return distances;
}

std::vector<double> box::calculate_gr_single_atom(const atom& central_atom, double const dr, const std::string& reference_atom_name) {
	const std::vector<size_t> distances = get_distances(central_atom, reference_atom_name, dr);
	const double average_number_density = static_cast<double>(atoms_[reference_atom_name].size()) / box_volume_;

	const size_t number_of_bins = static_cast<size_t>(r_max_ / dr) + 1;
	std::vector<double>r_gr_function(number_of_bins);

	for (size_t bin = 0; bin < number_of_bins; bin++) {
		const double dV = volume(static_cast<double>(bin + 1) * dr) - volume(static_cast<double>(bin) * dr);
		const double gr = static_cast<double>(distances[bin]) / dV / average_number_density;
		r_gr_function[bin]=gr;
	}
	return r_gr_function;
}

std::vector<double> box::calculate_gr(double const dr, const atom_name_pair& pair) {
	const std::vector<atom>& refs = atoms_[pair.reference];
	std::vector<double> average_gr;
	bool first = true;
	for (const atom& i : refs) {
		std::vector<double>atom_ppcf = calculate_gr_single_atom(i, dr, pair.reference);
		//először csak másoljuk, utána már összeadjuk az átlagoláshoz
		if (first) {
			average_gr = atom_ppcf;
			first = false;
		}
		else {
			std::transform(average_gr.begin(), average_gr.end(),
				atom_ppcf.begin(), average_gr.begin(), [](const double a, const double b) {
					return a + b;
				});
		}

	}

	//átlagolás, le kell minden gr értéket osztani a referencia atomok számával
	std::ranges::transform(average_gr, average_gr.begin(), [&refs](double a) {
		return a /= static_cast<double>(refs.size());
		});
	return average_gr;
}

std::vector<std::vector<double>> box::ppcf_matrix(const double dr) {

	std::vector<std::vector<double>> result;
	for (const auto pairs = ppcf_combinations(); const auto & current_pair:pairs) {

		auto gr_values = calculate_gr(dr, current_pair);

		//Először kell r is, de utána redundáns lenne,
		//minden gr-hez ugyanaz az r tartozik.
		if (result.empty()) {
			const size_t length = static_cast<size_t>(r_max_ / dr)+1;
			auto r_values = linspace(dr/2, length, dr);
			result.push_back(r_values);
			result.push_back(gr_values);
		}
		else {
			result.push_back(gr_values);
		}
	}
	return result;
}

bool box::is_unique_pair(const std::vector<atom_name_pair>& pairs, const atom_name_pair& pair) {
	for (const auto& i : pairs) {
		if (pair == i) {
			return false;
		}
	}
	return true;
}

std::vector<atom_name_pair> box::ppcf_combinations() {

	std::vector<std::string> atomtypes;
	atomtypes.reserve(atoms_.size());

	std::vector<atom_name_pair> combs;

	//végigiterálunk az atoms_ map-en, és kigyűjtjük az atomok neveit
	for (const auto& key : atoms_ | std::views::keys) {
		atomtypes.push_back(key);
	}

	//végigiterálunk az atomok nevein, és minden egyedi kombinációt kigyűjtünk
	for (const auto& first : atomtypes) {
		for (const auto& second : atomtypes) {
			if (atom_name_pair pair(first, second); is_unique_pair(combs, pair)) {
				combs.push_back(pair);
			}
		}
	}

	return combs;
}

std::vector<double> box::linspace(const double start, const size_t length, const double increment) {
	std::vector<double>sequence(length);
	for (size_t i =0;i<length;i++) {
		sequence[i]=start+i*increment; 
	}
	return sequence;
}
