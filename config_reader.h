#pragma once
#include <regex>
#include <string>

class config_reader {
public:
	std::string path;
	size_t number_of_files = 0;
	std::string prefix;
	size_t from = 0, to = INT_MAX;
	double dr = 0.01;
	size_t increment = 1;
	std::string output_file_name = "output";
	std::string line;

	//ha read_cfg beolvassa az értéket, akkor truek lesznek
	bool path_from_config = false;
	bool prefix_modified = false;
	bool from_modified = false;
	bool to_modified = false;
	bool dr_modified = false;
	bool increment_modified = false;
	bool output_name_modified = false;

	//regexek a config fájl beolvasásához
	std::regex path_regex = std::regex(R"(^\s*path\s*=\s*(\S+)\s*$)");
	std::regex prefix_regex = std::regex(R"(^\s*prefix\s*=\s*(\S+)\s*$)");
	std::regex from_to_regex = std::regex(R"(^\s*from-to\s*=.*$)");
	std::regex from_regex = std::regex(R"(^\s*from-to\s*=\s*([\d]+).*$)");
	std::regex to_regex = std::regex(R"(^\s*from-to\s*=\s*[\d]+\s*([\d]+|end)\s*$)");
	std::regex dr_regex = std::regex(R"(^\s*dr\s*=\s*(\d+(?:\.\d+)?)\s*$)");
	std::regex output_file_regex = std::regex(R"(^\s*output\s*name\s*=\s*(\S+)\s*$)");
	std::regex increment_regex = std::regex(R"(^\s*increment\s*=\s*(\d+)\s*$)");

	config_reader() { read_cfg_file(); }

	//megpróbálja beolvasni a config fájlból a beállításokat, a ctor ezt hívja meg
	void read_cfg_file();

	//segédfüggvenyei a read_cfg_file-nak
	void try_parse_path();
	void try_parse_prefix();
	void try_parse_from_to();
	void try_parse_dr();
	void try_parse_increment();
	void try_parse_output_name();
};

