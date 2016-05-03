#include "Utility.h"

const unsigned long Utility::Hash(const std::string &s, const int &mod) {
	unsigned long key = 0;
    for (auto c : s) {
        key = ((key << 5) + key) ^ c;
    }
    return key & ((1UL << mod) - 1);
}

const std::string Utility::TrimRight(const std::string &s) {
	auto trimmedStr = s;
	for (auto it = trimmedStr.rbegin(); it != trimmedStr.rend(); ++it) {
		if (*it == ' ') {
			trimmedStr.erase(std::next(it).base());
		} else {
			break;
		}
	}
	return trimmedStr;
}

const std::string Utility::ColorOut(const std::string &s, const Color &color) {
	std::string str;
	switch (color) {
		case Color::BLACK:
			str = "\033[30m";
			break;
		case Color::RED:
			str = "\033[31m";
			break;
		case Color::GREEN:
			str = "\033[32m";
			break;
		case Color::YELLOW:
			str = "\033[33m";
			break;
		case Color::BLUE:
			str = "\033[34m";
			break;
		case Color::MAGENTA:
			str = "\033[35m";
			break;
		case Color::CYAN:
			str = "\033[36m";
			break;
		case Color::WHITE:
			str = "\033[37m";
			break;
	}
	str += s + "\033[0m";
	return str;
}

const std::string Utility::ColorOut(const unsigned long &i, const Color &color) {
	return ColorOut(std::to_string(i), color);
}