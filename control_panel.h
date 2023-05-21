#pragma once
#include <filesystem>
#include <string>
#include <regex>
#include "box.h"
#include "box_reader.h"
#include "config_reader.h"

class control_panel
{

	//amit sikerül beolvasni a config.txt-ből felhasználja, a többit bekéri runtime
	//ha nem sikerült beolvasni rákérdez, hogy használja-e a default value-t, amit ki is írja hogy mi az
	config_reader generate_configuration() const;

	//generate_configuration() segédfüggvényei
	size_t count_files(config_reader conf) const;
	void read_from_to_from_user(config_reader& conf) const;
	static void read_dr_from_user(config_reader& conf);
	static void read_path_from_user(config_reader& conf);
	static void read_prefix_from_user(config_reader& conf);
	static void read_output_name_from_user(config_reader& conf);

	//
	void initiate_analysis(const config_reader& conf) const {
		namespace fs = std::filesystem;

		bool first = true;
		std::vector<std::vector<double>> ppcf;

		//TODO ez így már nem megy túl, ugye?
		int files_analysed = 0;
		for (const auto& entry : fs::directory_iterator(conf.path)) {
			if (entry.is_regular_file()) {
				std::ifstream file(entry.path());

				box_reader reader(entry.path().string());
				auto b = reader.read_from_file();

				std::vector<std::vector<double>>ppcf_from_single_file = b.ppcf_matrix(conf.dr);
				if (first) {
					first = false;
					ppcf = ppcf_from_single_file;
					files_analysed++;
				}
				else {
					std::transform(ppcf.begin(), ppcf.end(), ppcf_from_single_file.begin(), ppcf.begin(),
						[](std::vector<double>& col1, const std::vector<double>& col2) {
							std::vector<double> sum(col1.size());
							std::transform(col1.begin(), col1.end(), col2.begin(), col1.begin(),
								[](const double x, const double y) { return x + y; });
							return col2;
						}
					);
					files_analysed++;
				}
			}
		}

		std::transform(ppcf.begin(), ppcf.end(), ppcf.begin(), [files_analysed](std::vector<double>& col) {
			std::transform(col.begin(), col.end(), col.begin(), [files_analysed](double val) {
				return val /= files_analysed;
				});
			return col;
			});

		auto output_name = conf.output_file_name;
		output_name += ".pdb";
	}

};

