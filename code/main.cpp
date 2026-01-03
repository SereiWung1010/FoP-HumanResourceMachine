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

	vector<int> floor; // consider using an array
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
	vector<int> floor = lvl.floor;
	vector<bool> floorAvailable = lvl.floorAvailable;

	bool hasCurrentBlock = false;
	int currentBlock = 0;

	int n; //numOfInstruction
	while (true) {  // validate input
		cout << "Enter Amount of Desired Instruction : ";
		if (!(cin >> n) || n <= 0) {
		cout << "Please enter a positive number\n";
		cin.clear();
		cin.ignore(10000, '\n');
		}
		else {
			cin.ignore(10000, '\n'); // consume leftover newline
			break;
		}
	}

	vector<string> Instruction(n);
	
	// ===== read & validate instructions =====
	for (int i = 0; i < n; i++) {
		cout << i + 1 << " ";
		string cmd;
		int X = -1;
		
		cin >> cmd;
		to_lowercase(cmd);

		if (cmd == "add" || cmd == "sub" || cmd == "copyto" || cmd == "copyfrom" || cmd == "jump" || cmd == "jumpifzero") {
			if (!(cin >> X)) {    // read number argument
				cout << "Invalid argument for " << cmd << endl;
				return INSTRUCTION_ERROR;
			}
		}

		if (find(lvl.instructionAvailable.begin(),
				lvl.instructionAvailable.end(),
				cmd) == lvl.instructionAvailable.end()) { //find ( .. == .. ) will return false unless the instruction is inside the vector
			cout << "Error on instruction " << i + 1 << endl;
			return INSTRUCTION_ERROR;
		}

		Instruction[i] = cmd + (X != -1 ? " " + to_string(X) : "");
	}

    // ===== execute =====
	int ip = 0; // ip for instruction pointer
	while (ip < Instruction.size()) {
		string cmd = Instruction[ip];
		int X = -1; // 代表没 argument
		size_t pos = cmd.find(' '); // find the first space (ideally, 就 add 0) 就一个 space； 并返回 position
		if (pos != string::npos) { // there is a space; npos 代表 no space
			X = stoi(cmd.substr(pos + 1)); // stoi 可以理解为 s(string) to i(int); (..) 里面是 cin >> cmd 拿出 space 之后的 number string (这里需要 convert to int) 放到 X 里代表 argument
 			cmd = cmd.substr(0, pos); // 剩下 cmd 就是 0 到 space 之间的 string; 用 ().substr 来拉出
 		}

		if (cmd == "inbox") {
			if (input.empty()) {
                break;
            }
			currentBlock = input.front();
			input.erase(input.begin());
			hasCurrentBlock = true;
		}
		else if (cmd == "outbox") {
			if (!hasCurrentBlock) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			output.push_back(currentBlock);
			hasCurrentBlock = false;
		}
		else if (cmd == "add") {
			if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			currentBlock += floor[X];
		}
		else if (cmd == "sub") {
			if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			currentBlock -= floor[X];
		}
		else if (cmd == "copyto") {
			if(!hasCurrentBlock || X < 0 || X >= floor.size()) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			floor[X] = currentBlock;
			floorAvailable[X] = false;
		}
		else if (cmd == "copyfrom") {
			if (X < 0 || X >= floor.size() || floorAvailable[X]) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			currentBlock = floor[X];
			hasCurrentBlock = true;
		}
		else if (cmd == "jump") {
			if (X < 1 || X >= Instruction.size()) { // 这里至少有另外1个 instruction 才能 loop 回去
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			ip = X-1; //之后有 ip++ ，但只有不用 jump 时才会加，所以提前减之后加就扯平了
			continue;
		}
		else if (cmd == "jumpifzero") {
			if (!hasCurrentBlock || X < 1 || X >= Instruction.size()) { // no currentBlock to determine jump or not
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			} 
			if (currentBlock == 0) {
				ip = X-1;
				continue;
			}
		}
		ip++;
	}

	return ((output == lvl.expectedOutput && input.empty()) ? SUCCESS : FAIL);
}

int main() {

	// ============================ Intro Interface / Level Selection ========================= //

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

		if (!(cin >> selectLevel)) { 
			cout << "Please Enter a Number\n";
			cin.clear(); // 就是 clear
			cin.ignore(10000, '\n'); // 如果乱输入，可把前 10000 字符忽略 P.S 10000 既然在 continue 后还奏效？ 后续没法输入啊
			continue;
		}

		if (selectLevel < 1 || selectLevel > 4) {
			cout << "Level doesn't exist" << endl;
			continue;
		}

		if (selectLevel > lastLevel) {
			cout << "Level Locked" << endl;
			continue;
		}

		level currentLevel;
		// ============================ Level 1 =========================
		if (selectLevel == 1) {
			cout << "Level 1 :" << endl;

			currentLevel.input = {1, 2};
			currentLevel.expectedOutput = {1, 2};
			currentLevel.instructionAvailable = {"inbox", "outbox"};
			currentLevel.floor = {};
			currentLevel.floorAvailable = {};
		}

		// ============================ Level 2 =========================
		if (selectLevel == 2) {
			cout << "Level 2 :" << endl;

			currentLevel.input = {3, 9, 5, 1, -2, -2, 9, -9};
			currentLevel.expectedOutput = {-6, 6, 4, -4, 0, 0, 18, -18};
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {0,0,0};
			currentLevel.floorAvailable = {true, true, true};
		}

		// ============================ Level 3 =========================
		if (selectLevel == 3) {
			cout << "Level 3 :" << endl;

			currentLevel.input = {6, 2, 7, 7, -9, 3, -3, -3};
			currentLevel.expectedOutput = {7, -3};
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {0,0,0};
			currentLevel.floorAvailable = {true, true, true};
		}		

		// ============================ Level 4 =========================
		if (selectLevel == 4) {
			cout << "Level 4 :" << endl;

			currentLevel.input = {5, 3, 8, 2, -4};
			currentLevel.expectedOutput = {-2, 5, -6, -6};
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {0,0};
			currentLevel.floorAvailable = {true,true};
		}	

	LevelStatus result = playLevel(currentLevel);
	if (result == SUCCESS) {
		cout << "Success" << endl;
 		lastLevel = selectLevel + 1; //last level here 代表 not completed yet, you were here, now continue
	} else if (result == FAIL) {
		cout << "Fail" << endl;
	}
	}

	return 0;
}
