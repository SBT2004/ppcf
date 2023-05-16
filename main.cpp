#include "atom.h"
#include "box.h"
#include <filesystem>


int main() {
	namespace fs = std::filesystem;

	std::cout << "Specify the common part of filenames in the folder";
	std::string filename_root;
	std::cin >> filename_root;
	const std::string path = R"(D:\MD\ppcf_frame_test)";

	size_t file_count = 0;

	for (const auto& file : fs::directory_iterator(path)) {
		if (is_regular_file(file)) {
			file_count++;
		}
	}

	size_t from, to;
	std::cout << "Specify which files to analyze";
	std::cin >> from >> to;

	double dr;
	std::cout << "Specify the step size dr";
	std::cin >> dr;

	std::vector<std::vector<double>>average_ppcf;
	std::string filename = filename_root;

	for (size_t i = from; i <= to; i++) {
		filename += std::to_string(i);
		filename += ".pdb";
		auto b1 = box(filename);
		std::vector<std::vector<double>>ppcf = b1.ppcf_matrix(dr);
	}

}