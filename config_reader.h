#pragma once
#include <regex>
#include "config.h"

class config_reader {
private:
	//a config fájl mindegyik sorának megfelelő regex, config fájl beolvasásához
	std::regex path_regex = std::regex(R"(^\s*path\s*=\s*(\S+)\s*$)");
	std::regex prefix_regex = std::regex(R"(^\s*prefix\s*=\s*(\S+)\s*$)");
	std::regex from_to_regex = std::regex(R"(^\s*from-to\s*=.*$)");
	std::regex from_regex = std::regex(R"(^\s*from-to\s*=\s*([\d]+).*$)");
	std::regex to_regex = std::regex(R"(^\s*from-to\s*=\s*[\d]+\s*([\d]+|end)\s*$)");
	std::regex dr_regex = std::regex(R"(^\s*dr\s*=\s*(\d+(?:\.\d+)?)\s*$)");
	std::regex output_file_regex = std::regex(R"(^\s*output\s*name\s*=\s*(\S+)\s*$)");
	std::regex increment_regex = std::regex(R"(^\s*increment\s*=\s*(\d+)\s*$)");

	//segédfüggvényei a read_cfg_file-nak
	void try_parse_path(config& conf) const;
	void try_parse_prefix(config& conf) const;
	void try_parse_from_to(config& conf) const;
	void try_parse_dr(config& conf) const;
	void try_parse_increment(config& conf) const;
	void try_parse_output_name(config& conf) const;

public:
	config_reader() = default;

	//megpróbálja beolvasni a config fájlból a beállításokat, és feltölteni a config structot
	config read_cfg_file() const;
};
