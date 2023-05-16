#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <regex>
#include "box.h"
// a config file tartalma
class analyis_config {
public:
	std::string path;
	size_t number_of_files = 0;
	std::string prefix;
	//ide majd lesz egy end enum/logikai érték
	size_t from = 0, to = INT_MAX;
	double dr = 0.01;
	std::string output_file_name = "output";
	size_t increment = 1;
	analyis_config() :path(nullptr), prefix(nullptr) {
		read_cfg_file();
	}
	void read_cfg_file();
};

class control_panel
{
	//amit sikerül beolvasni a config.txt-ből felhasználja a többit bekéri runtime
	//ha nem sikerült beolvasni rákérdez, hogy használja-e a default value-t, amit ki is ír hogy mi az
	analyis_config generate_configuration() {
		namespace fs = std::filesystem;

		analyis_config conf;

		//ha nincs megadva, be lehetne olvasni
		std::cout << "Specify the prefix of the files";
		std::string prefix;
		std::cin >> prefix;
		conf.prefix = prefix;

		const std::string path = R"(D:\MD\ppcf_frame_test)";
		conf.path = path;

		//ezt nem kérjük be, de a to értékkel túl lehet menni
		size_t file_count = 0;
		for (const auto& file : fs::directory_iterator(path)) {
			if (is_regular_file(file)) {
				file_count++;
			}
		}
		conf.number_of_files = file_count;

		size_t from, to;
		std::cout << "Specify which files to analyze";
		std::cin >> conf.from >> conf.to;

		//ötlet config class, config.dr_given method
		double dr;
		std::cout << "Specify the step size dr";
		std::cin >> dr;
		conf.dr = dr;

		std::vector<std::vector<double>>average_ppcf;
		std::string output_file_name = prefix;


		return conf;
	}
	void initiate_analysis(const analyis_config conf) const {
		for (size_t i = conf.from; i <= conf.to; i++) {
			auto output_name = conf.output_file_name;
			output_name += std::to_string(i);
			output_name += ".pdb";
			auto b1 = box(output_name);
			std::vector<std::vector<double>>ppcf = b1.ppcf_matrix(conf.dr);
		}
	}

};

