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