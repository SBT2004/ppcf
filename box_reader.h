#pragma once
#include <filesystem>

#include "box.h"

class box_reader {
	const std::filesystem::path path_;

public:
	explicit box_reader(std::filesystem::path path): path_(std::move(path)) {  }

	box read_from_file() const;
};

