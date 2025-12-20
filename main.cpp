#include <iostream>
#include <vector> // vector
#include <string> // string
#include <cctype> // tolower()
#include <algorithm> //find()
using namespace std;

enum LevelStatus { SUCCESS, FAIL, INSTRUCTION_ERROR };

struct level {
	vector<int> input;
	vector<int> expectedOutput;

	vector<int> floor;
	vector<bool> floorAvailable;

	vector<string> instructionAvailable;
};

void to_lowercase(string &s) {
	for (char &c : s) {
		c = tolower(static_cast<unsigned char>(c));
	}
};

LevelStatus playLevel(const level &lvl) {
	vector<int> input = lvl.input;   
	vector<int> output;

	bool hasBlock = false;
	int currentBlock = 0;

	int n; //numOfInstruction
	cout << "Enter Amount of Desired Instruction : ";
	cin >> n;

	vector<string> Instruction(n);

	// ===== read & validate instructions =====
	for (int i = 0; i < n; i++) {
		cout << i + 1 << " ";
		cin >> Instruction[i];
		// cin.ignore(); (1. 12/17-2:13-ZJS) Initially, I thought I need this when I input "Input          " and it returns "Error..."; turns out it's just my typo
		to_lowercase(Instruction[i]);

		if (find(lvl.instructionAvailable.begin(),
				lvl.instructionAvailable.end(),
				Instruction[i]) == lvl.instructionAvailable.end()) { //find ( .. == .. ) will return false unless the instruction is inside the vector
			cout << "Error on instruction " << i + 1 << endl;
			return INSTRUCTION_ERROR;
		}
	}

    // ===== execute =====
	for (string &cmd : Instruction) {
		if (cmd == "inbox") {
			if (input.empty()) {
				cout << "Error! Input Chain is Empty\n";
				return FAIL;
			}
			currentBlock = input.front();
			input.erase(input.begin());
			//cout << "inbox -> current " << currentBlock << endl;
			hasBlock = true;
		}
		else if (cmd == "outbox") {
			if (!hasBlock) {
				cout << "Error! No Current Block\n";
				return FAIL;
			}
			output.push_back(currentBlock);
			//cout << "current -> outbox " << output.back() << endl;
			// (3. 12/17-2:51-ZJS) 本来用 LevelOne.output[i]， 忘了 i 不能通用， size 不一样; 这种 struct+vector 最高是 .begin() .back()
			hasBlock = false;
		}
		else if (cmd == "sub") {
			if (!hasBlock || input.empty()) {
				cout << "Error! SUB failed\n";
				return FAIL;
			}
			currentBlock -= input.front();
			input.erase(input.begin());
		}
	}

	// ===== verify ===== (old code)
	// LevelOne.success = true; 
	// if (LevelOne.output.size() != LevelOne.expectedOutput.size()) {
	// 	LevelOne.success = false;
	// } 
	// for (int i = 0; i < LevelOne.output.size(); i++) {
	// 	if (LevelOne.output[i] != LevelOne.expectedOutput[i]) {
	// 		LevelOne.success = false;   // mismatch found
	// 		break;
	// 	}
	// }
	// ===== verify ===== 
	return ((output == lvl.expectedOutput && input.empty()) ? SUCCESS : FAIL);
}

int main() {

	// ========================= 1 === Intro Interface / Level Selection ========================= //

	cout << "\n\n";
	cout << "\t" << "+-------------------------------------------------------+" << endl;
	cout << "\t" << "|                                                       |" << endl;
	cout << "\t" << "|                Human Resource Machine                 |" << endl;
	cout << "\t" << "|                                                       |" << endl;
	cout << "\t" << "+-------------------------------------------------------+" << endl;

	cout << "\t" << "  _____  " << "\t" << "  _____  " << "\t" << "  _____  " << "\t" << "  _____  " << endl;
	cout << "\t" << " [     | " << "\t" << " |     | " << "\t" << " |     | " << "\t" << " |     | " << endl;
	cout << "\t" << "_______|_" << "\t" << "_|_____|_" << "\t" << "_|_____|_" << "\t" << "_|_____|_" << endl;
	cout << "\t" << "|       |" << "\t" << "|       |" << "\t" << "|       |" << "\t" << "|       |" << endl;
	cout << "\t" << "|   1   |" << "\t" << "|   2   |" << "\t" << "|   3   |" << "\t" << "|   4   |" << endl;
	cout << "\t" << "|_______|" << "\t" << "|_______|" << "\t" << "|_______|" << "\t" << "|_______|" << endl;

	int selectLevel;
	int lastLevel = 1;   // Later update with data store in file;

	while (true) {
		cout << "\nPlease Select Your Level : ";
		cin >> selectLevel;
		// ===== verify Level Completiton Status =====
		// cout << ((selectLevel > lastLevel)? "Level Locked" : 0 ); // Atp 1. I don't have else, so I put 0 as dummy exp; Atp 2. OMG, I forgot that we have else, AND IT'S THE CODE FOR THE GAME WE ARE GONNA PLAY
		if (selectLevel > lastLevel) {
			cout << "Level Locked" << endl;
			continue;
		}

		// ========================= 3 === Level 1 ========================= //
		if (selectLevel == 1) {
			cout << "Level 1 :" << endl;

			level LevelOne;
			LevelOne.input = {1, 2};
			LevelOne.expectedOutput = {1, 2};
			LevelOne.instructionAvailable = {"inbox", "outbox"};
			LevelOne.floor = {};
			LevelOne.floorAvailable = {};

			//cout << (LevelOne.success ? "SUCCESS" : "FAIL"); 
			LevelStatus result = playLevel(LevelOne);
			if (result == SUCCESS) {
				cout << "SUCCESS" << endl;
				lastLevel = 2;
			} else if (result == FAIL) {
				cout << "FAIL" << endl;
			}
		}

		// ========================= 4 === Level 2 ========================= //
		// if (selectLevel == 2) {
		// 	cout << "Level 2 :" << endl;

		// 	level LevelOne;
		// 	LevelOne.input = {1, 2};
		// 	LevelOne.expectedOutput = {1, 2};
		// 	LevelOne.instructionAvailable = {"inbox", "outbox"};
		// 	LevelOne.floor = {};
		// 	LevelOne.floorAvailable = {};

		// 	//cout << (LevelOne.success ? "SUCCESS" : "FAIL"); 
		// 	LevelStatus result = playLevel(LevelOne);
		// 	if (result == SUCCESS) {
		// 		cout << "SUCCESS" << endl;
		// 		lastLevel = 2;
		// 	} else if (result == FAIL) {
		// 		cout << "FAIL" << endl;
		// 	}
		// }






		// ========================= 5 === Level 3 ========================= //
		// ========================= 6 === Level 4 ========================= //
	}
	return 0;
}
