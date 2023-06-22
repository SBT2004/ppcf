#pragma once
#include <sstream>
#include <string>

inline bool is_integer(const std::string& str) {
	std::stringstream ss(str);
	int value;
	return (ss >> value) && ss.eof();
}

inline int to_integer(const std::string& str) {
	std::stringstream ss(str);
	int value;
	ss >> value;
	return value;
}

inline bool is_end_flag(const std::string& str) {
	return str == "end";
}

inline bool is_double(const std::string& str) {
	bool has_dot = false;
	for (const char c : str) {
		if (!isdigit(c)) {
			if (c == '.' && !has_dot) {
				has_dot = true;
			}
			else return false;
		}
	}
	return true;
}

inline bool is_word(const std::string& input_string) {
	for (const auto& c : input_string) {
		if (!std::isalpha(c)) {
			return false;
		}
	}
	return true;
}