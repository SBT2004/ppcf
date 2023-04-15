#pragma once
#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>

#include "Atom.h"
#include <unordered_map>

class box
{
	//a boxban atomokat tárolunk, emellett a kiterjedését,
	//a szimulációban eltelt időt ns-ban, és hogy ez hanyadik frame
	std::unordered_map<std::string, std::vector<atom>> atoms_;
	size_t time_;
	size_t frame_number_;
	struct dimensions {
		double x;
		double y;
		double z;
		//dimensions() = delete;
		explicit dimensions(std::smatch& matches);

	};
	dimensions dim_;


	explicit box(std::ifstream file_state_pointer/*char* current_line*/) {
		std::ifstream file("example.txt");

		//ATOM      1  OW  SOL     1      21.650   1.220   1.310  1.00  0.00           O
		std::regex reg_atom(R"(^ATOM\s+\d+\s+(\w+)\s+\w+\s+\d+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

		//TITLE     SPCE water t= 200.00000 step= 100000
		std::regex reg_frame(R"(^TITLE.+t=\s*(\d*\.\d+)\s+step=\s*(\d+))");

		//CRYST1   22.758   22.758   22.758  90.00  90.00  90.00 P 1           1
		std::regex reg_dim(R"(^\w+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

		if (file.is_open()) {
			std::string line;
			std::smatch matches;
			while (std::getline(file, line)/*&&if statement*/) {
				if (std::regex_search(line, matches, reg_atom)) {
					atoms_[std::string(matches[0])].push_back(atom(matches));
				}
				else if (std::regex_search(line, matches, reg_frame)) {
					time_ = std::stoi(matches[0]);
					frame_number_ = std::stoi(matches[1]);
				}
				else if (std::regex_search(line, matches, reg_dim)) {
					dim_ = dimensions(matches);
				}
			}
		}
		else {
			//exception kell majd ide
			std::cout << "Failed to open file" << std::endl;
		}
		//csak alá kell rakni, mert megszünteti a statet
		file.close();
	}

	//std::vector<size_t> index(const std::string& name);
	constexpr double average_number_density_ = dim_.x * dim_.y * dim_.z * M_PI * 4 / 3;
	std::vector<double> get_distances(const atom& a, const std::string& type2);

	void ppcf_single_atom(const atom& a, double const dr, const std::string& type2);

	//végigmegy az összes atomon és minden új atomtípust feljegyez
	/*std::vector<std::string> get_names() const;*/

	static double volume(const double r) { return 4 * r * r * r * M_PI / 3; }


	//megállapítja milyen fajta ppcf-ek szükségesek,
	//hogy minden atom mindennel legyen párosítva,
	//viszont a sorrend mindegy AB BA ugyanaz, így abból csak egy kell


};


