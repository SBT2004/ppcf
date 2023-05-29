#pragma once
#include <filesystem>
#include "box.h"

//a doboz adatait beolvassa egy fájlból
class box_reader {
	const std::filesystem::path path_;

public:
//meg kell adni az elérési utat, ahol az input fájlok vannak
	explicit box_reader(std::filesystem::path path): path_(std::move(path)) {  }

//beolvassa az adatokat egy fájlból, majd feltölti vele a box példányt, amit visszaad
	box read_from_file() const;
};

