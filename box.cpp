#include "box.h"
//std::vector<size_t> box::index(const std::string& name) {
//	std::vector<size_t> index_set;
//	int j=0;
//	for (size_t i=0;i<atoms_.size();i++) {
//		if (atoms_[i].getname() == name) {
//			index_set[j++] = i;
//		}
//	}
//	return index_set;
//}

box::dimensions::dimensions(const std::smatch& matches) {
	x = std::stod(matches[1]);
	y = std::stod(matches[2]);
	z = std::stod(matches[3]);
}

std::vector<double> box::get_distances(const atom& a, const std::string& type2) {
	auto range = atoms_.equal_range(type2);
	std::vector<double> distances;
	int j = 0;
	for (auto i : range)
	{
		distances[j++] = a.distance(atoms_[i]);
	}
	std::sort(distances.begin(), distances.end());
	return distances;
}

void box::ppcf_single_atom(const atom& a, double const dr, const std::string& type2) {

	auto distances = get_distances(a,type2);

	for (double r = dr; r < dim_.x; r += dr) {
		double dV = volume(r + dr) - volume(r);
		const auto lower = std::lower_bound(distances.begin(), distances.end(), r);
		const auto upper = std::upper_bound(distances.begin(), distances.end(), r + dr);
		//bugprone narrowing conversion?
		int count = std::distance(lower, upper);
		double gr = (count/dV)/average_number_density_;
			
	}
}

//std::vector<std::string> box::get_names() const {
//	std::vector<std::string> strings;
//	int j = 0;
//	for (const atom& i : atoms_)
//	{
//		auto it = std::find(strings.begin(), strings.end(), i.getname());
//		if (it != strings.end()) {
//			strings[j++] = i.getname();
//		}
//	}
//	return strings;
//}
