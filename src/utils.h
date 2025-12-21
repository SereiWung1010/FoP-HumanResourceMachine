#ifndef UTILS_H
#define UTILS_H

#include <string> // string
#include <cctype> // tolower()

void to_lowercase(std::string &s) {
	for (char &c : s) {
		c = tolower(static_cast<unsigned char>(c));
	}
};

#endif