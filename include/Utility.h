#ifndef CHORD_SIMULATION_INCLUDE_UTILITY_H_INCLUDED
#define CHORD_SIMULATION_INCLUDE_UTILITY_H_INCLUDED

#include <string>

enum class Color {

	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE

};

namespace Utility {

	const unsigned long Hash(const std::string &s, const int &mod);

	const std::string TrimRight(const std::string &s);

	const std::string ColorOut(const std::string &s, const Color &color);

	const std::string ColorOut(const unsigned long &i, const Color &color);

}

#endif