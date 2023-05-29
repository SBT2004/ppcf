#pragma once
#include <filesystem>
#include "box.h"

//a doboz adatait beolvassa egy f�jlb�l
class box_reader {
	const std::filesystem::path path_;

public:
//meg kell adni az el�r�si utat, ahol az input f�jlok vannak
	explicit box_reader(std::filesystem::path path): path_(std::move(path)) {  }

//beolvassa az adatokat egy f�jlb�l, majd felt�lti vele a box p�ld�nyt, amit visszaad
	box read_from_file() const;
};

