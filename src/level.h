#ifndef LEVEL_H
#define LEVEL_H

#include <vector> // vector
#include <string> // string
using namespace std;

enum LevelStatus { SUCCESS, FAIL, INSTRUCTION_ERROR };

struct level {
	vector<int> input;
	vector<int> expectedOutput;

	vector<int> floor; // consider using an array
	vector<bool> floorAvailable;

	vector<string> instructionAvailable;
    int currentLevel;

};

#endif // LEVEL_H