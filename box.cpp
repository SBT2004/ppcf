#include "box.h"

box::dimensions::dimensions(const double x_in, const double y_in, const double z_in) : x(x_in), y(y_in), z(z_in) {}

box::box(const std::string& path) : time_(NAN), frame_number_(NAN) {

	std::ifstream file(path);

	//ATOM      1  OW  SOL     1      21.650   1.220   1.310  1.00  0.00           O
	std::regex reg_atom(R"(^ATOM\s+\d+\s+(\w+)\s+\w+\s+\d+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

	//TITLE     SPCE water t= 200.00000 step= 100000
	std::regex reg_frame(R"(^TITLE.+t=\s*(\d*\.\d+)\s+step=\s*(\d+))");

	//CRYST1   22.758   22.758   22.758  90.00  90.00  90.00 P 1           1
	std::regex reg_dim(R"(^\w+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

	//ezen érdemes elgondolokdni
	//If n == 0, returns a reference to the std::sub_match representing the part of the target sequence matched by the entire matched regular expression.
	if (file.is_open()) {
		std::string line;
		std::smatch matches;
		while (std::getline(file, line)) {
			if (std::regex_search(line, matches, reg_atom)) {
				atoms_[std::string(matches[0])].push_back(atom(stod(matches[1]), stod(matches[2]), stod(matches[3])));
			}
			else if (std::regex_search(line, matches, reg_frame)) {
				time_ = std::stoi(matches[0]);
				frame_number_ = std::stoi(matches[1]);
			}
			else if (std::regex_search(line, matches, reg_dim)) {
				dim_ = dimensions(stod(matches[0]), stod(matches[1]), stod(matches[2]));
			}
		}
		file.close();
	}
	else {
		throw std::invalid_argument("Failed to open file");
	}
	if (atoms_.empty()) {
		throw std::invalid_argument("Can't read atoms from file");
	}
}

std::vector<double> box::get_distances(const atom& a, const std::string& type2) {
	std::vector<double> distances;
	int j = 0;
	for (atom& i : atoms_[type2])
	{
		distances.push_back(a.distance(i, this->dim_.x));
	}
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

		double dV = volume(r + dr) - volume(r);
		rgr.gr = (count / dV) / average_number_density_;
		res.push_back(rgr);
	}
	return res;
}

std::vector<ppcf_values> box::calculate_ppcf(double const dr, const ppcf_pair& pair) {
	const std::vector<atom>refs = atoms_[pair.reference];
	std::vector<ppcf_values> average_ppcf;
	for (const atom& i : refs) {
		std::vector<ppcf_values>atom_ppcf = calculate_ppcf_single_atom(i, dr, pair.reference);
		std::transform(average_ppcf.begin(), average_ppcf.end(),
			atom_ppcf.begin(), average_ppcf.begin(), [](ppcf_values a, const ppcf_values b) {
				a.gr += b.gr;
				return a;
			});
	}
	std::transform(average_ppcf.begin(), average_ppcf.end(), average_ppcf.begin(), [&refs](ppcf_values a) {
		a.gr /= refs.size();
		return a;
		});
	return average_ppcf;
}

std::vector<std::vector<double>> box::ppcf_matrix(const double dr) {

	std::vector<std::vector<double>> result;
	for (const std::vector<ppcf_pair>pairs = ppcf_combinations(); const auto & current_pair:pairs) {

		auto ppcf_function = calculate_ppcf(dr, current_pair);
		std::vector<double>gr_values;

		//először kell r is, de utána redundáns lenne
		if (result.empty()) {
			std::vector<double>r_values;
			std::transform(ppcf_function.begin(), ppcf_function.end(), r_values.begin(), [](const ppcf_values a) {return a.r; });
			std::transform(ppcf_function.begin(), ppcf_function.end(), gr_values.begin(), [](const ppcf_values a) {return a.gr; });
		}
		std::transform(ppcf_function.begin(), ppcf_function.end(), gr_values.begin(), [](const ppcf_values a) {return a.gr; });
		result.push_back(gr_values);
	}
	return result;
}

bool box::is_unique_pair(const std::vector<ppcf_pair>& pairs, const ppcf_pair& pair) {
	for (const auto& i : pairs) { if (pair == i) { return false; } }
	return true;
}

std::vector<box::ppcf_pair> box::ppcf_combinations() {

	std::vector<std::string> atomtypes;
	std::vector<ppcf_pair> combs;
	for (const auto& key : atoms_ | std::views::keys) { atomtypes.push_back(key); }

	for (const auto& first : atomtypes) {
		for (const auto& second : atomtypes) {
			if (ppcf_pair pair(first, second); is_unique_pair(combs, pair)) {
				combs.push_back(pair);
			}
		}
	}

	return combs;
}
