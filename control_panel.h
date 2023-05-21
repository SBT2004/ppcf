#pragma once
#include <filesystem>
#include "config_reader.h"

//TODO: kéne neki jobb hely
std::ofstream& operator<<(std::ofstream& ofs, const std::vector<std::vector<double>>& ppcf);
class control_panel
{

	//amit sikerül beolvasni a config.txt-ből felhasználja, a többit bekéri runtime
	//ha nem sikerült beolvasni rákérdez, hogy használja-e a default value-t, amit ki is írja hogy mi az
	config_reader generate_configuration() const;

	//generate_configuration() segédfüggvényei
	size_t count_files(const config_reader& conf) const;
	void read_from_to_from_user(config_reader& conf) const;
	static void read_dr_from_user(config_reader& conf);
	static void read_path_from_user(config_reader& conf);
	static void read_prefix_from_user(config_reader& conf);
	static void read_output_name_from_user(config_reader& conf);

	//kiszámolja minden fájlra a ppcf értéket, majd ezeket átlagolja
	std::vector<std::vector<double>> calculate_average_ppcf(const config_reader& conf) const;
public:
	//elvégzi a számolást, a config_reader-ből kiolvassa a szükséges adatokat
	void initiate_analysis() const;
};