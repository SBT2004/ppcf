#include "box_reader.h"

#include <regex>
#define PCRE2_CODE_UNIT_WIDTH 16

#include <pcre2.h>
#include <stdexcept>

#include "atom.h"
#include "box.h"
#include "dimensions.h"

box box_reader::read_from_file() const {

	//ATOM      1  OW  SOL     1      21.650   1.220   1.310  1.00  0.00           O
	std::regex reg_atom(R"(^ATOM\s+\d+\s+(\w+)\s+\w+\s+\d+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

	auto reg_atom_str = (PCRE2_SPTR)R"(^ATOM\s+\d+\s+(\w+)\s+\w+\s+\d+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))";
	 int errorcode;
    PCRE2_SIZE erroroffset;

	auto re = pcre2_compile(reg_atom_str, PCRE2_ZERO_TERMINATED, 0, 
		&errorcode,                // Error code
        &erroroffset,              // Error offset,
		NULL);

	pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);


	//TITLE     SPCE water t= 200.00000 step= 100000
	std::regex reg_frame(R"(^TITLE.+t=\s*(\d*\.\d+)\s+step=\s*(\d+))");

	//CRYST1   22.758   22.758   22.758  90.00  90.00  90.00 P 1           1
	std::regex reg_dim(R"(^\w+\s+(\d*\.\d+)\s+(\d*\.\d+)\s+(\d*\.\d+))");

	std::ifstream file(path_);

	if (file.is_open()) {
		std::unordered_map<std::string, std::vector<atom>> atoms;
		size_t time = INT_MAX;
		size_t frame_number = INT_MAX;
		dimensions dim;

		std::string line;
		std::smatch matches;
		while (std::getline(file, line)) {

		    int rc = pcre2_match(
	        re,                        // Compiled pattern
	        (PCRE2_SPTR)line.c_str(),                   // Subject string
	        PCRE2_ZERO_TERMINATED,     // Length of the subject string
	        0,                         // Start offset
	        0,                         // Match options
	        match_data,                // Match data
	        NULL                       // Match context
		    );


			if (std::regex_search(line, matches, reg_atom)) {
				atoms[std::string(matches[1])].push_back(atom(stod(matches[2]), stod(matches[3]), stod(matches[4])));
			}
			else if (std::regex_search(line, matches, reg_frame)) {
				time = std::stoi(matches[1]);
				frame_number = std::stoi(matches[2]);
			}
			else if (std::regex_search(line, matches, reg_dim)) {
				dim = dimensions(stod(matches[1]), stod(matches[2]), stod(matches[3]));
			}
		}
		file.close();

		if (atoms.empty() || time == INT_MAX || frame_number == INT_MAX) {
			throw std::invalid_argument("Invalid file content");
		}
		return box(atoms, dim, time, frame_number);
	}

	throw std::invalid_argument("Failed to open file");
}
