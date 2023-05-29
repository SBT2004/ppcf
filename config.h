#pragma once
#include <string>

struct config {
	std::string path;
	size_t number_of_files = 0;
	std::string prefix;
	size_t from = 0, to = INT_MAX;
	double dr = 0.01;
	size_t increment = 1;
	std::string output_file_name = "output";
	std::string line;

	//ha config_reader::read_cfg beolvassa az értéket, akkor truek lesznek
	bool path_from_config = false;
	bool prefix_from_config = false;
	bool from_from_config = false;
	bool to_from_config = false;
	bool dr_from_config = false;
	bool increment_from_config = false;
	bool output_name_from_config = false;
};

