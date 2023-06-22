#pragma once
#include <filesystem>

#include "box.h"

class box_reader {
	const std::filesystem::path path_;

public:
	explicit box_reader(std::filesystem::path path) : path_(std::move(path)) {  }

	box read_from_file() const;

	static std::vector<std::string> try_parse_coordinates_and_name_from_line(const std::string& line);

	std::vector<double> try_parse_time_and_frame_from_line(const std::string& line) const;

	std::vector<double>try_parse_box_dimensions_from_line(const std::string& line) const;
};

