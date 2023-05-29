#include "config_reader.h"
#include <fstream>
#include "string_utils.h"

config config_reader::read_cfg_file() const {
	if (std::ifstream file("config.txt"); file.is_open()) {
		config conf;
		//beolvassuk soronként a config fájlt és megpróbáljuk feltölteni a config osztály mezőit
		while (std::getline(file, conf.line)) {
			try_parse_path(conf);
			try_parse_prefix(conf);
			try_parse_from_to(conf);
			try_parse_dr(conf);
			try_parse_increment(conf);
			try_parse_output_name(conf);
		}
		file.close();
		return conf;
	}
	throw std::invalid_argument("Failed to open config file");
}

void config_reader::try_parse_path(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, path_regex)) {
		conf.path = matches[1].str();
		conf.path_from_config = true;
	}
}
void config_reader::try_parse_prefix(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, prefix_regex)) {
		conf.prefix = matches[1].str();
		conf.prefix_from_config = true;
	}
}

//to és from szét van szedve, először to van kezelve, majd from
//ha to nem szám hanem "end", annak külön esete van, ekkor az utolsó fájlig megyünk
void config_reader::try_parse_from_to(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, from_to_regex)) {
		if (std::regex_match(conf.line, matches, from_regex)) {
			conf.from = std::stoi(matches[1].str());
			conf.from_from_config = true;
		}
		if (std::regex_match(conf.line, matches, to_regex)) {
			if (is_integer(matches[1])) {
				conf.to = std::stoi(matches[1].str());
			}
			conf.to_from_config = true;
		}
	}
}

void config_reader::try_parse_dr(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, dr_regex)) {
		conf.dr = std::stod(matches[1].str());
		conf.dr_from_config = true;
	}
}

void config_reader::try_parse_increment(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, output_file_regex)) {
		conf.output_file_name = matches[1].str();
		conf.increment_from_config = true;
	}
}

void config_reader::try_parse_output_name(config& conf) const {
	std::smatch matches;
	if (std::regex_match(conf.line, matches, increment_regex)) {
		conf.increment = std::stoi(matches[1].str());
		conf.output_name_from_config = true;
	}
}