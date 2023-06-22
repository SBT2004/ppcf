#include "box_reader.h"

#include <regex>
#include <stdexcept>

#include "atom.h"
#include "box.h"
#include "dimensions.h"
#include "string_utils.h"

box box_reader::read_from_file() const {

	if (std::ifstream file(path_); file.is_open()) {
		std::unordered_map<std::string, std::vector<atom>> atoms;
		size_t time = INT_MAX;
		size_t frame_number = INT_MAX;
		dimensions dim;

		std::string line;
		while (std::getline(file, line)) {

			if (line.find("ATOM") != std::string::npos) {
				if (std::vector<std::string> search_result = try_parse_coordinates_and_name_from_line(line); !search_result.empty())
					atoms[search_result[0]].emplace_back(
						atom(std::stod(search_result[1]), std::stod(search_result[2]), std::stod(search_result[3])));
			}
			else if (line.find("TITLE") != std::string::npos) {

				if (auto search_result = try_parse_time_and_frame_from_line(line); !search_result.empty()) {
					time = static_cast<size_t>(search_result[0]);
					frame_number = static_cast<size_t>(search_result[1]);
				}
			}
			else if (line.find("CRYST1") != std::string::npos) {

				if (auto search_result = try_parse_box_dimensions_from_line(line); !search_result.empty()) {
					dim = dimensions(search_result[0], search_result[1], search_result[2]);
				}
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

std::vector<std::string> box_reader::try_parse_coordinates_and_name_from_line(const std::string& line) {
	//ATOM      1  OW  SOL     1      21.650   1.220   1.310  1.00  0.00           O

	// Splitting the line into tokens based on whitespace
	std::istringstream iss(line);
	std::string token;
	std::vector<std::string> tokens;

	//the max number of tokens should be 8
	for (int n = 0; iss >> token && n < 8; n++) {
		tokens.push_back(token);
	}

	std::vector<std::string> data = {};

	//if the syntax is not valid return an empty vector
	if (!is_double(tokens[1])) return data;
	if (!is_word(tokens[3]))   return data;
	if (!is_double(tokens[4])) return data;

	// Parsing the required numbers from the tokens after the checks
	data.push_back(tokens[2]);
	data.push_back(tokens[5]);
	data.push_back(tokens[6]);
	data.push_back(tokens[7]);
	return data;
}

std::vector<double> box_reader::try_parse_time_and_frame_from_line(const std::string& line) const {
	//TITLE     SPCE water t= 200.00000 step= 100000
	std::istringstream iss(line);
	std::string token;
	std::vector<std::string> tokens;

	size_t time_location = 0;
	size_t frame_location = 0;
	int i = 0;
	while (iss >> token) {
		i++;
		tokens.push_back(token);
		if (token == "t=") time_location = i;
		else if (token == "step=") frame_location = i;
	}

	std::vector<double> time_and_frame = {};
	if (time_location != 0 && frame_location != 0) {
		time_and_frame.push_back(std::stod(tokens[time_location]));
		time_and_frame.push_back(std::stod(tokens[frame_location]));
	}
	return time_and_frame;
}

std::vector<double> box_reader::try_parse_box_dimensions_from_line(const std::string& line) const {
	//CRYST1   22.758   22.758   22.758  90.00  90.00  90.00 P 1           1

	// Splitting the line into tokens based on whitespace
	//can be later extended to non cube boxes, the next 3 numbers represent the angles between the sides
	std::istringstream iss(line);
	std::string token;
	std::vector<std::string> tokens;

	//the max number of tokens should be 8
	for (int n = 0; iss >> token && n < 4; n++) {
		tokens.push_back(token);
	}
	const auto x = std::stod(tokens[1]);
	const auto y = std::stod(tokens[2]);
	const auto z = std::stod(tokens[3]);
	std::vector coords = { x,y,z };
	return coords;
}