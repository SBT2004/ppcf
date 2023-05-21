#include "box.h"
std::vector<double> box::get_distances(const atom& a, const std::string& type2) {
	std::vector<double> distances;
	distances.reserve(atoms_[type2].size());

	//végigiterálunk a type2 keyyel rendelkező vektoron
	for (const auto& i : atoms_[type2])
	{
		distances.push_back(a.distance(i, dim_.x));
	}
	//fontos, hogy be legyen rendezve, mert a lower_bound és upper_bound csak rendezett vektoron működik
	std::sort(distances.begin(), distances.end());
	return distances;
}

std::vector<ppcf_values> box::calculate_ppcf_single_atom(const atom& a, double const dr, const std::string& type2) {

	auto distances = get_distances(a, type2);
	std::vector<ppcf_values>res;
	const double max_r = dim_.x / 2;

	for (double r = dr; r < max_r; r += dr) {
		ppcf_values rgr;
		rgr.r = r;

		//distances rendezve tárolja center távolságokat
		const auto lower = std::lower_bound(distances.begin(), distances.end(), r);
		const auto upper = std::upper_bound(distances.begin(), distances.end(), r + dr);
		const size_t count = std::distance(lower, upper);

		const double dV = volume(r + dr) - volume(r);

		rgr.gr = (static_cast<double>(count) / dV) / average_number_density_;
		res.push_back(rgr);
	}
	return res;
}

std::vector<ppcf_values> box::calculate_ppcf(double const dr, const atom_name_pair& pair) {
	const std::vector<atom>refs = atoms_[pair.reference];
	std::vector<ppcf_values> average_ppcf;
	average_ppcf.reserve(refs.size());

	for (const atom& i : refs) {
		std::vector<ppcf_values>atom_ppcf = calculate_ppcf_single_atom(i, dr, pair.reference);
		std::transform(average_ppcf.begin(), average_ppcf.end(),
			atom_ppcf.begin(), average_ppcf.begin(), [](ppcf_values a, const ppcf_values b) {
				a.gr += b.gr;
				return a;
			});
	}

	std::transform(average_ppcf.begin(), average_ppcf.end(), average_ppcf.begin(), [&refs](ppcf_values a) {
		a.gr /= static_cast<double>(refs.size());
		return a;
		});
	return average_ppcf;
}

std::vector<std::vector<double>> box::ppcf_matrix(const double dr) {

	std::vector<std::vector<double>> result;
	for (const std::vector<atom_name_pair>pairs = ppcf_combinations(); const auto & current_pair:pairs) {

		auto ppcf_function = calculate_ppcf(dr, current_pair);
		std::vector<double>gr_values;

		//először kell r is, de utána redundáns lenne
		if (result.empty()) {
			std::vector<double>r_values(ppcf_function.size());
			std::transform(ppcf_function.begin(), ppcf_function.end(), r_values.begin(), [](const ppcf_values a) {
				return a.r;
				});
			std::transform(ppcf_function.begin(), ppcf_function.end(), gr_values.begin(), [](const ppcf_values a) {
				return a.gr;
				});
		}
		std::transform(ppcf_function.begin(), ppcf_function.end(), gr_values.begin(), [](const ppcf_values a) {
			return a.gr;
			});
		result.push_back(gr_values);
	}
	return result;
}

bool box::is_unique_pair(const std::vector<atom_name_pair>& pairs, const atom_name_pair& pair) {
	for (const auto& i : pairs) { if (pair == i) { return false; } }
	return true;
}

std::vector<atom_name_pair> box::ppcf_combinations() {

	std::vector<std::string> atomtypes;
	atomtypes.reserve(atoms_.size());

	std::vector<atom_name_pair> combs;
	for (const auto& key : atoms_ | std::views::keys) { atomtypes.push_back(key); }

	for (const auto& first : atomtypes) {
		for (const auto& second : atomtypes) {
			if (atom_name_pair pair(first, second); is_unique_pair(combs, pair)) {
				combs.push_back(pair);
			}
		}
	}

	return combs;
}
