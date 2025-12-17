#include <iostream>
#include <vector> // vector
#include <string> // string
#include <cctype> // tolower()
using namespace std;

struct level {
	vector<int> input;
	vector<int> output;
	vector<int> expectedOutput;
};

void to_lowercase(string &s) {
    for (char &c : s) {
        c = tolower(static_cast<unsigned char>(c));
    }
};

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
	int lastLevel = 1; // Later update with data store in file;
	do {
		cout << "\n" << "Please Select Your Level : ";
		cin >> selectLevel;
		// ========================= 2 === Verify Level Completion Status ========================= //
		// cout << ((selectLevel > lastLevel)? "Level Locked" : 0 ); // Atp 1. I don't have else, so I put 0 as dummy exp; Atp 2. OMG, I forgot that we have else, AND IT'S THE CODE FOR THE GAME WE ARE GONNA PLAY
		if (selectLevel > lastLevel) cout << "Level Locked" ;
		else {

	// ========================= 3 === Level 1 ========================= //
			cout << "Level 1 : " << endl;
			int currentBlock = -1; //(2. 12/17-2:46-ZJS) 本来是 = 0； 但后续考虑到有些积木也是 0， 所以就换成 -1
			level LevelOne;
			LevelOne.input = {1,2};
			LevelOne.expectedOutput = {1,2};
			int numOfInstruction;
			cout << "Enter Amount of Desired Instruction : ";
			cin >> numOfInstruction;

			vector<string> Instruction(numOfInstruction);
			for(int i = 0; i < numOfInstruction; i++) {
				cout << i+1 << " ";
				cin >> Instruction[i];
				// cin.ignore(); (1. 12/17-2:13-ZJS) Initially, I thought I need this when I input "Input          " and it returns "Error..."; turns out it's just my typo
				to_lowercase(Instruction[i]); 
				if (Instruction[i] != "inbox" && Instruction[i] != "outbox") {
					cout << "Error on instruction " << i+1; 
					break;
				}
			}
		
			for(int i = 0; i < numOfInstruction; i++) {
				if (Instruction[i] == "inbox") {
					if (LevelOne.input.size() == 0) {
						cout << "Error! Input Chain is Empty";
						break;
					}
					else {
						currentBlock = LevelOne.input.front();
						LevelOne.input.erase(LevelOne.input.begin());
						cout << "inbox -> current " << currentBlock << endl;
					}
				}
				else if (Instruction[i] == "outbox") {
					if (currentBlock == -1) {
						cout << "Error! No Current Block";
						break;
					} 
					else {
						LevelOne.output.push_back(currentBlock);
						cout << "current -> outbox " << LevelOne.output.back() << endl; // (3. 12/17-2:51-ZJS) 本来用 LevelOne.output[i]， 忘了 i 不能通用， size 不一样; 这种 struct+vector 最高是 .begin() .back()
					}
				}
			}
			//verifying result
				if (LevelOne.output.size() != LevelOne.expectedOutput.size()) {
					cout << "FAIL";
					break;
				} 
				for (int i = 0; i < LevelOne.output.size(); i++) {
					if (LevelOne.output[i] != LevelOne.expectedOutput[i]) {
						cout << "FAIL";   // mismatch found
						break;
					}
					else {
						cout << "SUCCESS";
						break;
					}
				}
		}
	} while(selectLevel > lastLevel);

	return 0;
}