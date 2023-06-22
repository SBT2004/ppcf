#include "box.h"
#include <algorithm>

std::vector<double> box::get_distances(const atom& central_atom, const std::string& reference_atom_name) {
	std::vector<double> distances;

	//telítmény optimalizálás, hogy ne kelljen növelgetni memóriát
	distances.reserve(atoms_[reference_atom_name].size());

	//végigiterálunk a reference_atom_name key-jel rendelkező vektoron
	for (const auto& i : atoms_[reference_atom_name]) {
		auto distance = central_atom.distance(i, dim_.x);
		distances.push_back(distance);
	}

	//fontos, hogy be legyen rendezve, mert a lower_bound és upper_bound csak rendezett vektoron működik
	std::ranges::sort(distances);

	return distances;
}

std::vector<double> box::calculate_gr_single_atom(const atom& central_atom, double const dr, const std::string& reference_atom_name) {
	auto distances = get_distances(central_atom, reference_atom_name);
	std::vector<double>res;
	const double average_number_density = static_cast<double>(atoms_[reference_atom_name].size()) / box_volume_;

	for (double r = 0; r < r_max_; r += dr) {
		double gr;

		//distances rendezve tárolja center távolságokat
		std::vector<double>::iterator up;

		const auto lower = std::ranges::lower_bound(distances, r);
		for (auto it = lower; it != distances.end(); ++it) {
			if (*it >= r + dr) {
				up = it;
				break;
			}
		}

		const size_t counts = std::distance(lower, up);

		const double dV = volume(r + dr) - volume(r);

		gr = (static_cast<double>(counts) / dV) / average_number_density;
		res.push_back(gr);
	}
	return res;
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
			auto r_values = linspace(0, r_max_, static_cast<int>(r_max_ / dr) + 1);
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

std::vector<double> box::linspace(const double start, const double end, const int num_points) {
	std::vector<double> sequence;
	sequence.reserve(num_points);

	const double step_size = (end - start) / (num_points - 1.0);
	double value = start;

	for (int i = 0; i < num_points; ++i) {
		sequence.push_back(value);
		value += step_size;
	}

	return sequence;
}