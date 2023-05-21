#include "config_reader.h"
#include <fstream>
#include "string_utils.h"

void config_reader::read_cfg_file() {
	if (std::ifstream file("config.txt"); file.is_open()) {
		//beolvassuk soronként a config fájlt és megpróbáljuk feltölteni a config_reader osztály mezőit
		while (std::getline(file, line)) {
			try_parse_path();
			try_parse_prefix();
			try_parse_from_to();
			try_parse_dr();
			try_parse_increment();
			try_parse_output_name();
		}
		file.close();
	}

	else {
		throw std::invalid_argument("Failed to open config file");
	}
}

void config_reader::try_parse_path() {
	std::smatch matches;
	if (std::regex_match(line, matches, path_regex)) {
		path = matches[1].str();
		path_from_config = true;
	}
}
void config_reader::try_parse_prefix() {
	std::smatch matches;
	if (std::regex_match(line, matches, prefix_regex)) {
		prefix = matches[1].str();
		prefix_modified = true;
	}
}

//to és from szét van szedve, először to van kezelve, majd from
//ha to nem szám hanem end, annak külön esete van, ekkor az utolsó fájlig megyünk
void config_reader::try_parse_from_to() {
	std::smatch matches;
	if (std::regex_match(line, matches, from_to_regex)) {
		if (std::regex_match(line, matches, from_regex)) {
			from = std::stoi(matches[1].str());
			from_modified = true;
		}
		if (std::regex_match(line, matches, to_regex)) {
			if (is_integer(matches[1])) {
				to = std::stoi(matches[1].str());
			}
			to_modified = true;
		}
	}
}

void config_reader::try_parse_dr() {
	std::smatch matches;
	if (std::regex_match(line, matches, dr_regex)) {
		dr = std::stod(matches[1].str());
		dr_modified = true;
	}
}

void config_reader::try_parse_increment() {
	std::smatch matches;
	if (std::regex_match(line, matches, output_file_regex)) {
		output_file_name = matches[1].str();
		increment_modified = true;
	}
}

void config_reader::try_parse_output_name() {
	std::smatch matches;
	if (std::regex_match(line, matches, increment_regex)) {
		increment = std::stoi(matches[1].str());
		output_name_modified = true;
	}
}