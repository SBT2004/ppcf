#include "control_panel.h"

#include <iostream>

#include "config_reader.h"
#include "string_utils.h"

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
size_t control_panel::count_files(config_reader conf) const {
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

	std::string from, to;
	size_t pos = input.find(' ');
	from = input.substr(0, pos);
	to = input.substr(pos);

	if (is_integer(from)) {
		conf.from = to_integer(from);
	}

	if (is_integer(to)) {
		conf.to = to_integer(to);
	}

	else if (is_end_flag(to)) {}
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