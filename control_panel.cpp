#include <iostream>
#include "config_reader.h"
#include "string_utils.h"
#include "control_panel.h"

#include "box_reader.h"

config_reader control_panel::generate_configuration() const {

	config_reader conf;

	if (!conf.prefix_modified) {
		read_prefix_from_user(conf);
	}
	if (!conf.path_from_config) {
		read_path_from_user(conf);
	}
	if (!conf.from_modified) {
		read_from_to_from_user(conf);
	}
	if (!conf.dr_modified) {
		read_dr_from_user(conf);
	}
	if (!conf.path_from_config) {
		read_output_name_from_user(conf);
	}
	//a felhaszálónak nem kell tudnia, hogy hány fájl van, a program számolja meg
	conf.number_of_files = count_files(conf);

	return conf;
}
size_t control_panel::count_files(const config_reader& conf) const {
	size_t file_count = 0;
	for (const auto& file : std::filesystem::directory_iterator(conf.path)) {
		if (is_regular_file(file)) {
			file_count++;
		}
	}
	return file_count;
}

void control_panel::read_from_to_from_user(config_reader& conf) const {
	std::string input;
	std::cout << "Enter from to values: ";
	std::getline(std::cin, input);

	const size_t pos = input.find(' ');
	const std::string from = input.substr(0, pos);
	const std::string to = input.substr(pos);

	if (is_integer(from)) {
		conf.from = to_integer(from);
	}

	if (is_integer(to)) {
		conf.to = to_integer(to);
	}

	else if (is_end_flag(to)) {/*INT_MAX az alapérték, alapesetben is a végéig menne*/ }
}

void control_panel::read_dr_from_user(config_reader& conf) {
	char ans;
	std::cout << "No dr value was provided, use default value? y/n?";
	std::cin >> ans;
	if (ans == 'n') {
		double dr;
		std::cout << "Specify the step size dr";
		std::cin >> dr;
		conf.dr = dr;
	}
}

void control_panel::read_path_from_user(config_reader& conf) {
	std::cout << "Specify the path of the files";
	std::string path;
	std::cin >> path;
	conf.path = path;
}

void control_panel::read_prefix_from_user(config_reader& conf) {
	std::cout << "Specify the prefix of the files";
	std::string prefix;
	std::cin >> prefix;
	conf.prefix = prefix;
}

void control_panel::read_output_name_from_user(config_reader& conf) {
	std::string output_file_name;
	std::cout << "Specify the name of the output file";
	std::cin >> output_file_name;
	conf.output_file_name = output_file_name;
}

std::vector<std::vector<double>> control_panel::calculate_average_ppcf(const config_reader& conf) const {
	namespace fs = std::filesystem;

	bool first = true;
	std::vector<std::vector<double>> ppcf;
	const size_t max_files_to_be_analysed = (conf.to - conf.from + 1) / conf.increment;


	size_t files_analysed = 0;
	auto it = fs::directory_iterator(conf.path);
	while (it != fs::directory_iterator() && files_analysed < max_files_to_be_analysed) {
		auto& entry = *it;
		box_reader reader(entry.path());
		auto b = reader.read_from_file();

		std::vector<std::vector<double>>ppcf_from_single_file = b.ppcf_matrix(conf.dr);

		if (first) {
			first = false;
			ppcf = ppcf_from_single_file;
			files_analysed++;
		}
		else {
			if (ppcf.size() < ppcf_from_single_file.size()) {
				ppcf.resize(ppcf_from_single_file.size(), std::vector<double>(ppcf_from_single_file[0].size()));
			}
			for (size_t i = 0; i < std::min(ppcf.size(), ppcf_from_single_file.size()); i++) {
				for (size_t j = 0; j < std::min(ppcf[i].size(), ppcf_from_single_file[i].size()); j++) {
					ppcf[i][j] += ppcf_from_single_file[i][j];
				}
			}
			files_analysed++;
		}
		std::advance(it, conf.increment);
	}
	for (auto& i : ppcf) {
		for (double& j : i) {
			j /= static_cast<double>(files_analysed);
		}
	}
	return ppcf;
}

void control_panel::initiate_analysis() const {
	const config_reader& conf = generate_configuration();
	const std::vector<std::vector<double>>ppcf = calculate_average_ppcf(conf);
	auto output_name = conf.output_file_name;
	output_name += ".pdb";
	std::ofstream output_file(output_name);
	//TODO: kell neki valamiért az iostream
	output_file << ppcf;
}

std::ofstream& operator<<(std::ofstream& ofs, const std::vector<std::vector<double>>& ppcf) {
	if (ppcf.empty()) {
		return ofs;
	}
	for (size_t i = 0; i < ppcf[0].size(); i++) {
		for (const auto& column : ppcf) {
			ofs << column[i] << " ";
		}
		ofs << std::endl;
	}
	return ofs;
}
