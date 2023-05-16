#include "control_panel.h"
void analyis_config::read_cfg_file() {
	//std::regex reg_path(R"(^\s*path\s*=\s*\b([\w\\.-:]+)\b\s*$)");
	//std::regex reg_prefix(R"(^\s*prefix\s*=\s*\b([\w]+)\b\s*$)");
	//std::regex reg_from_to(R"(^\s*from-to\s*=\s*([\d]+)\s*([\d]+)\s*$)");
	//std::regex reg_dr(R"(^\s*dr\s*=\s*([\d].*[\d]+)\s*$)");
	//std::regex reg_output_name(R"(^\s*output\s*name\s*=\s*\b([\w]+)\b\s*$)");
	//std::regex reg_increment(R"(^\s*increment\s*=\s*([\d]+)\s*$)");

	std::ifstream file("config.txt");
	if (file.is_open()) {
		std::string line;
		//beolvassuk soronként a config fájlt és feltöltjük a config osztály mezőit, ha sikerül
		while (std::getline(file, line)) {
			std::smatch matches;

			if (std::regex_match(line, matches, std::regex(R"(^\s*path\s*=\s*(\S+)\s*$)"))) {
				path = matches[1].str();
				path_modified = true;
			}
			else if (std::regex_match(line, matches, std::regex(R"(^\s*prefix\s*=\s*(\S+)\s*$)"))) {
				prefix = matches[1].str();
				prefix_modified = true;
			}

			//to és from szét van szedve, először to van kezelve, majd from
			//ha to nem szám hanem end, akkor annak külön esete van, ekkor az utolsó fájlig megyünk
			else if (std::regex_match(line, matches, std::regex(R"(^\s*from-to\s*=.*$)"))) {
				if (std::regex_match(line, matches, std::regex(R"(^\s*from-to\s*=\s*([\d]+).*$)"))) {
					from = std::stoi(matches[1].str());
					from_modified = true;
				}
				else if (std::regex_match(line, matches, std::regex(R"(^\s*from-to\s*=\s*[\d]+\s*([\d]+)\s*$)"))) {
					to = std::stoi(matches[1].str());
					to_modified = true;
				}
				else if (std::regex_match(line, matches, std::regex(R"(^\s*from-to\s*=\s*[\d]+\s*end\s*$)"))) {
					//to-nak kéne valami temp érték, ami jelzi hogy end flag van.
					to = end;
					to_modified = true;
					to_end_flag = true;
				}

			}
			else if (std::regex_match(line, matches, std::regex(R"(^\s*dr\s*=\s*(\d+(?:\.\d+)?)\s*$)"))) {
				dr = std::stod(matches[1].str());
				dr_modified = true;
			}
			else if (std::regex_match(line, matches, std::regex(R"(^\s*output\s*name\s*=\s*(\S+)\s*$)"))) {
				output_file_name = matches[1].str();
				increment_modified = true;
			}
			else if (std::regex_match(line, matches, std::regex(R"(^\s*increment\s*=\s*(\d+)\s*$)"))) {
				increment = std::stoi(matches[1].str());
				output_name_modified = true;
			}
			file.close();
		}
	}
	else {
		throw std::invalid_argument("Failed to open config file");
	}
}
