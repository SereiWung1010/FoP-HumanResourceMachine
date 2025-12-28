#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
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
    for (char &c : s) c = tolower(static_cast<unsigned char>(c));
}
 
LevelStatus playLevel(const level &lvl, const vector<string> &Instruction) {
    vector<int> input = lvl.input;
    vector<int> output;
    vector<int> floor = lvl.floor;
    vector<bool> floorAvailable = lvl.floorAvailable;
 
    bool hasCurrentBlock = false;
    int currentBlock = 0;
 
    int ip = 0;
    while (ip < Instruction.size()) {
        string cmd = Instruction[ip];
        int X = -1;
 
        size_t pos = cmd.find(' ');
        if (pos != string::npos) {
            try {
                X = stoi(cmd.substr(pos + 1));
            } catch (...) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            cmd = cmd.substr(0, pos);
        }
 
        if (find(lvl.instructionAvailable.begin(),
                 lvl.instructionAvailable.end(),
                 cmd) == lvl.instructionAvailable.end()) {
            cout << "Error on instruction " << ip + 1 << endl;
            return INSTRUCTION_ERROR;
        }
 
        if (cmd == "inbox") {
            if (input.empty()) {
                if(output == lvl.expectedOutput){
                    return SUCCESS;
                }
                else return FAIL;
            }
            currentBlock = input.front();
            input.erase(input.begin());
            hasCurrentBlock = true;
        } else if (cmd == "outbox") {
            if (!hasCurrentBlock) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            output.push_back(currentBlock);
            hasCurrentBlock = false;
        } else if (cmd == "add") {
            if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            currentBlock += floor[X];
        } else if (cmd == "sub") {
            if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            currentBlock -= floor[X];
        } else if (cmd == "copyto") {
            if (!hasCurrentBlock || X < 0 || X >= floor.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            floor[X] = currentBlock;
            floorAvailable[X] = false;
        } else if (cmd == "copyfrom") {
            if (X < 0 || X >= floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            currentBlock = floor[X];
            hasCurrentBlock = true;
        } else if (cmd == "jump") {
            if (X < 1 || X > Instruction.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            ip = X - 1;
            continue;
        } else if (cmd == "jumpifzero") {
            if (!hasCurrentBlock || X < 1 || X > Instruction.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }
            if (currentBlock == 0) {
                ip = X - 1;
                continue;
            }
        }
        ip++;
    }
 
    return (output == lvl.expectedOutput && input.empty()) ? SUCCESS : FAIL;
}
 
int main() {
    int selectLevel, m;
    cin >> selectLevel >> m;
    cin.ignore(); 
 
    vector<string> Instruction(m);
    for (int i = 0; i < m; i++) {
        getline(cin, Instruction[i]);
        // to_lowercase(Instruction[i]);
    }
 
    level currentLevel;
    if (selectLevel == 1) {
        currentLevel.input = {1, 2};
        currentLevel.expectedOutput = {1, 2};
        currentLevel.instructionAvailable = {"inbox", "outbox"};
    } else if (selectLevel == 2) {
        currentLevel.input = {3, 9, 5, 1, -2, -2, 9, -9};
        currentLevel.expectedOutput = {-6, 6, 4, -4, 0, 0, 18, -18};
        currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
        currentLevel.floor = {0,0,0};
        currentLevel.floorAvailable = {true,true,true};
    } else if (selectLevel == 3) {
        currentLevel.input = {6, 2, 7, 7, -9, 3, -3, -3};
        currentLevel.expectedOutput = {7, -3};
        currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
        currentLevel.floor = {0,0,0};
        currentLevel.floorAvailable = {true,true,true};
    }
 
    LevelStatus result = playLevel(currentLevel, Instruction);
    if (result == SUCCESS) cout << "Success" << endl;
    else if (result == FAIL) cout << "Fail" << endl;
 
    return 0;
}