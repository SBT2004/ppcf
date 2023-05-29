#pragma once
#include <filesystem>
#include "config_reader.h"

std::ofstream& operator<<(std::ofstream& ofs, const std::vector<std::vector<double>>& ppcf);

//Összefogja a program működéséhez szükséges függvényeket és osztályokat, delegálja a config beolvasását,
//a számolást és a kiírást.
class control_panel
{
private:
	//Amit sikerül beolvasni a config.txt-ből felhasználja, a többit bekéri a felhasználótól.
	//Ha nem sikerült beolvasni rákérdez, hogy használja-e a default value-t, amit kiír, hogy micsoda.
	static config generate_configuration();

	//generate_configuration() segédfüggvényei
	static size_t count_files(const config& conf);
	static void read_from_to_from_user(config& conf);
	static void read_dr_from_user(config& conf) ;
	static void read_path_from_user(config& conf);
	static void read_prefix_from_user(config& conf);
	static void read_output_name_from_user(config& conf);

	//kiszámolja minden fájlra a ppcf értéket, majd ezeket átlagolja
	std::vector<std::vector<double>> calculate_average_ppcf(const config& conf) const;
public:
	//elvégzi a számolást, a config_reader-ből kiolvassa a szükséges adatokat
	void initiate_analysis() const;
};