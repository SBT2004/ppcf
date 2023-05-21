#pragma once
#include <string>

//ppcf_values számításhoz egy központi atomot és egy referencia atomot tároló struct
	struct atom_name_pair {
		std::string center;
		std::string reference;

		atom_name_pair(std::string str_a, std::string str_b) : center(std::move(str_a)), reference(std::move(str_b)) {}

		//ha egy pár sorrendtől függtetlenül megegyezik true, pl. AB=BA
		bool operator==(const atom_name_pair& other) const;
	};

