#include <iostream>
#include "config_reader.h"
#include "string_utils.h"
#include "control_panel.h"

#include "box_reader.h"

config control_panel::generate_configuration() {
	const config_reader reader;
	config conf = reader.read_cfg_file();

	if (!conf.prefix_from_config) {
		read_prefix_from_user(conf);
	}
	if (!conf.path_from_config) {
		read_path_from_user(conf);
	}
	if (!conf.from_from_config) {
		read_from_to_from_user(conf);
	}
	if (!conf.dr_from_config) {
		read_dr_from_user(conf);
	}
	if (!conf.path_from_config) {
		read_output_name_from_user(conf);
	}
	//a felhaszálónak nem kell tudnia, hogy hány fájl van, a program számolja meg
	conf.number_of_files = count_files(conf);

	return conf;
}

size_t control_panel::count_files(const config& conf) {
	size_t file_count = 0;
	for (const auto& file : std::filesystem::directory_iterator(conf.path)) {
		if (is_regular_file(file)) {
			file_count++;
		}
	}
	return file_count;
}

void control_panel::read_from_to_from_user(config& conf) {
	std::cout << "No from and to values were provided, do you want to read all files? y/n? ";
	if (std::cin.get() == 'n') {
		std::cin.ignore();
		std::cout << "Enter from to values: ";
		std::string input;
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
}

void control_panel::read_dr_from_user(config& conf) {
	char ans;
	std::cout << "No dr value was provided, use default value (0,01)? y/n? ";
	std::cin >> ans;
	if (ans == 'n') {
		double dr;
		std::cout << "Specify the step size dr ";
		std::cin >> dr;
		conf.dr = dr;
	}
}

void control_panel::read_path_from_user(config& conf) {
	std::cout << "Specify the path of the files ";
	std::string path;
	std::cin >> path;
	conf.path = path;
}

void control_panel::read_prefix_from_user(config& conf) {
	std::cout << "Specify the prefix of the files ";
	std::string prefix;
	std::cin >> prefix;
	conf.prefix = prefix;
}

void control_panel::read_output_name_from_user(config& conf) {
	char ans;
	std::cout << "No output file name was provided, use default value (output)? y/n? ";
	std::cin >> ans;
	if (ans == 'n') {
		std::string output_file_name;
		std::cout << "Specify the name of the output file";
		std::cin >> output_file_name;
		conf.output_file_name = output_file_name;
	}

}

std::vector<std::vector<double>> control_panel::calculate_average_ppcf(const config& conf) const {
	namespace fs = std::filesystem;

	std::vector<std::vector<double>> ppcf;
	const size_t max_files_to_be_analysed = (conf.to - conf.from + 1) / conf.increment;

	size_t files_analysed = 0;
	bool first = true;
	auto it = fs::directory_iterator(conf.path);
	while (it != fs::directory_iterator() && files_analysed < max_files_to_be_analysed) {
		auto& entry = *it;
		box_reader reader(entry.path());
		auto current_box = reader.read_from_file();

		std::vector<std::vector<double>>ppcf_from_single_file = current_box.ppcf_matrix(conf.dr);

		//átlagot fogunk számolni,így az első elemet csak be kell másolni
		//a első fájl mérete jó referencia lesz, nem kell majd sokat bővíteni
		if (first) {
			first = false;
			ppcf = ppcf_from_single_file;
			files_analysed++;
		}
		else {
			//összegezzük a gr értékeket az átlagoláshoz
			for (size_t i = 0; i < ppcf.size(); i++) {
				for (size_t j = 0; j < std::min(ppcf[i].size(), ppcf_from_single_file[i].size()); j++) {
					ppcf[i][j] += ppcf_from_single_file[i][j];
				}
			}
			files_analysed++;
		}
		std::advance(it, conf.increment);
	}

	//átlagolás
	for (auto& i : ppcf) {
		for (double& j : i) {
			j /= static_cast<double>(files_analysed);
		}
	}
	return ppcf;
}

void control_panel::initiate_analysis() const {
	const config& conf = generate_configuration();
	const std::vector<std::vector<double>>ppcf = calculate_average_ppcf(conf);
	const auto output_name = conf.output_file_name + ".pdb";
	std::ofstream output_file(output_name);
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
