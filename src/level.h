#ifndef LEVEL_H
#define LEVEL_H

#include <vector> // vector
#include <string> // string

enum LevelStatus { SUCCESS, FAIL, INSTRUCTION_ERROR };

struct level {
	std::vector<int> input;
	std::vector<int> expectedOutput;

	std::vector<int> floor; // consider using an array
	std::vector<bool> floorAvailable;

	std::vector<std::string> instructionAvailable;
    int currentLevel;

};

#endif // LEVEL_H