#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

enum LevelStatus { SUCCESS, FAIL, INSTRUCTION_ERROR };

struct level {
	std::string instruction;
	std::vector<int> input;
	std::vector<int> correctOutput;
	std::vector<int> expectedOutput;
	std::vector<int> floor;
	std::vector<bool> floorAvailable;
	std::vector<std::string> instructionAvailable;
    int currentLevel;

};

#endif