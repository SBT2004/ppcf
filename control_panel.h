#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <regex>
#include "box.h"
// a config file tartalma
enum { end };
class analyis_config {
public:
	std::string path;
	size_t number_of_files = 0;
	std::string prefix;
	//ide majd lesz egy end enum/logikai érték
	size_t from = 0, to = INT_MAX;
	double dr = 0.01;
	size_t increment = 1;
	std::string output_file_name = "output";

	//ha read_cfg beolvassa az értéket, akkor truek lesznek
	bool path_modified = false;
	bool prefix_modified = false;
	bool from_modified = false;
	bool to_modified = false;
	bool dr_modified = false;
	bool increment_modified = false;
	bool output_name_modified = false;
	//ez akkor lesz true, ha a configban to értéknek end van megadva
	bool to_end_flag = false;


	analyis_config() { read_cfg_file(); }

	void read_cfg_file();

};

class control_panel
{
	//amit sikerül beolvasni a config.txt-ből felhasználja, a többit bekéri runtime
	//ha nem sikerült beolvasni rákérdez, hogy használja-e a default value-t, amit ki is ír hogy mi az
	analyis_config generate_configuration() {
		namespace fs = std::filesystem;

		analyis_config conf;

		if (!conf.prefix_modified) {
			std::cout << "Specify the prefix of the files";
			std::string prefix;
			std::cin >> prefix;
			conf.prefix = prefix;
		}
		if (!conf.path_modified) {
			std::cout << "Specify the path of the files";
			std::string path;
			std::cin >> path;
			conf.path = path;
		}

		//ezt nem kérjük be
		size_t file_count = 0;
		for (const auto& file : fs::directory_iterator(conf.path)) {
			if (is_regular_file(file)) {
				file_count++;
			}
		}
		conf.number_of_files = file_count;

		//TODO itt egybe kéne from-to beolvasás cinről
		if (!conf.from_modified) {
			size_t from;
			std::cout << "Specify which files to analyze";
			std::cin >> conf.from;
		}

		if (!conf.dr_modified) {
			double dr;
			std::cout << "Specify the step size dr";
			std::cin >> dr;
			conf.dr = dr;
		}

		if (!conf.path_modified) {
			std::string output_file_name;
			std::cout << "Specify the name of the output file";
			std::cin >> output_file_name;
			conf.output_file_name = output_file_name;
		}
		return conf;
	}

	void initiate_analysis(const analyis_config& conf) const {
		//TODO nem 1 increment esetén ne menjen túl
		for (size_t i = conf.from; i <= conf.to; i += conf.increment) {
			auto output_name = conf.output_file_name;
			output_name += std::to_string(i);
			output_name += ".pdb";
			auto b1 = box(output_name);
			std::vector<std::vector<double>>ppcf = b1.ppcf_matrix(conf.dr);
		}
	}

};

