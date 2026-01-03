#include "game_logic.h"
#include "utils.h"

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

// Only changed function signature - added UI& ui parameter
LevelStatus playLevel(const level &lvl, UI& ui) {
    vector<int> input = lvl.input;
    vector<int> output;
    vector<int> floor = lvl.floor;
    vector<bool> floorAvailable = lvl.floorAvailable;

    bool hasCurrentBlock = false;
    int currentBlock = -10000;

    int n; // numOfInstruction
    while (true) {  // validate input
        cout << "Enter Amount of Desired Instruction : ";
        if (!(cin >> n) || n <= 0) {
            cout << "Please enter a positive number\n";
            cin.clear();
            cin.ignore(10000, '\n');
        } else {
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
                 cmd) == lvl.instructionAvailable.end()) {
            cout << "Error on instruction " << i + 1 << endl;
            return INSTRUCTION_ERROR;
        }

        Instruction[i] = cmd + (X != -1 ? " " + to_string(X) : "");
    }

    // ===== 设置UI初始状态 =====
    UI::GameState uiState;
	uiState.instruction = lvl.instruction;
    uiState.input = input;
    uiState.output = {};
    uiState.field = floor;
    uiState.code = Instruction;
    uiState.currentBlock = currentBlock;
    uiState.currentCommand = 0;
    uiState.level = lvl.currentLevel;
    ui.setGameState(uiState);

    ui.clearScreen();
    ui.displayUI();

    // ===== execute =====
    int ip = 0; // instruction pointer
    while (ip < (int)Instruction.size()) {
        // ===== 在执行指令前更新UI =====
        ui.setCurrentCommand(ip);
        ui.clearScreen();
        ui.displayUI();

        string full = Instruction[ip];
        string cmd = full;
        int X = -1;

        size_t pos = cmd.find(' ');
        if (pos != string::npos) {
            X = stoi(cmd.substr(pos + 1));
            cmd = cmd.substr(0, pos);
        }

        if (cmd == "inbox") {
            if (input.empty()) {
                break;
            }

			currentBlock = input.front();
            input.erase(input.begin());
            hasCurrentBlock = true;

            ui.clearScreen();
            ui.moveTo(-1);
            ui.setInput(input);
            ui.setCurrentBlock(currentBlock);
        }
        else if (cmd == "outbox") {
            if (!hasCurrentBlock) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            output.insert(output.begin(), currentBlock);
            hasCurrentBlock = false;

            ui.clearScreen();
            ui.moveTo(4);
            ui.setOutput(output);
			ui.setCurrentBlock(-10000);
        }
        else if (cmd == "add") {
            if (!hasCurrentBlock || X < 0 || X >= (int)floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            currentBlock += floor[X];

            ui.setCurrentBlock(currentBlock);
            ui.clearScreen();
            ui.moveTo(X);
        }
        else if (cmd == "sub") {
            if (!hasCurrentBlock || X < 0 || X >= (int)floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            currentBlock -= floor[X];

            ui.clearScreen();
            ui.moveTo(X);
            ui.setCurrentBlock(currentBlock);
        }
        else if (cmd == "copyto") {
            if (!hasCurrentBlock || X < 0 || X >= (int)floor.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            floor[X] = currentBlock;
            floorAvailable[X] = false;

            ui.clearScreen();
            ui.moveTo(X);
            ui.setField(floor);
        }
        else if (cmd == "copyfrom") {
            if (X < 0 || X >= (int)floor.size() || floorAvailable[X]) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            currentBlock = floor[X];
            hasCurrentBlock = true;

            ui.clearScreen();
            ui.moveTo(X);
            ui.setCurrentBlock(currentBlock);
        }
        else if (cmd == "jump") {
            // Instructions are 1-indexed in input, 0-indexed in array
            if (X < 1 || X > (int)Instruction.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            this_thread::sleep_for(chrono::milliseconds(500));

            ip = X - 1;
            continue;
        }
        else if (cmd == "jumpifzero") {
            if (!hasCurrentBlock || X < 1 || X > (int)Instruction.size()) {
                cout << "Error on instruction " << ip + 1 << endl;
                return INSTRUCTION_ERROR;
            }

            if (currentBlock == 0) {

                this_thread::sleep_for(chrono::milliseconds(500));

                ip = X - 1;
                continue;
            }
        }
        else {
            // If an unknown instruction slips through (shouldn't), treat as error.
            cout << "Error on instruction " << ip + 1 << endl;
			this_thread::sleep_for(chrono::seconds(2));
            return INSTRUCTION_ERROR;
        }

        ip++;
    }

	ui.moveTo(0);

    // ===== verify =====
    if (output == lvl.expectedOutput && input.empty()) {
        ifstream inputFile("lastLevel.txt");
        if (inputFile) {
            int lastLevel;
            while (inputFile >> lastLevel) {
                if (lastLevel < lvl.currentLevel + 1) {
                    ofstream outputFile("lastLevel.txt");
                    outputFile << (lvl.currentLevel + 1);
                    outputFile.close();
                }
            }
            inputFile.close();
            return SUCCESS;
        } else {
            cerr << "A file could not be created. Please try again later.";
            return FAIL;
        }
    } else {
        return FAIL;
    }
}
