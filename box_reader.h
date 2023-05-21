#pragma once
#include "box.h"

class box_reader {
	const std::string& path_;

public:
	explicit box_reader(const std::string& path): path_(path) {  }

	box read_from_file() const;
};

