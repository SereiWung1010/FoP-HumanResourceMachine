#include "UI.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
using namespace std;

UI::UI() {}

void UI::clearScreen() {
    cout << "\33c\e[3J" << flush;
}

void UI::setGameState(const GameState& newState) {
    state = newState;
}

void UI::setInput(const std::vector<int>& input) {
    state.input = input;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(chrono::milliseconds(500));
}

void UI::setOutput(const std::vector<int>& output) {
    state.output = output;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(chrono::milliseconds(500));
}

void UI::setField(const vector<int>& field) {
    state.field = field;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(std::chrono::milliseconds(500));
}

void UI::setCode(const vector<string>& code) {
    state.code = code;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(chrono::milliseconds(500));
}

void UI::setCurrentBlock(int currentBlock) {
    state.currentBlock = currentBlock;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(chrono::milliseconds(500));
}

void UI::setCurrentCommand(int currentCommand) {
    state.currentCommand = currentCommand;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    std::this_thread::sleep_for(chrono::milliseconds(500));
}

void UI::setLevel(int level) {
    state.level = level;
    this_thread::sleep_for(chrono::milliseconds(500));
    clearScreen();
    displayUI();
    this_thread::sleep_for(chrono::milliseconds(500));
}

vector<string> UI::returnCommandContent(int max) {
    vector<string> commandContent(max, " ");

    for (int i = 0; i < max && i < static_cast<int>(state.code.size()); ++i) {
        if (i == state.currentCommand) {
            commandContent[i] = "> " + state.code[i];
        } else {
            commandContent[i] = "  " + (state.code[i]);
        }
    }

    return commandContent;
} 

string UI::formatInt(int value) {
    string s;

    if (value == -100) {
        s = "   ";
    } else { 
        s = to_string(value);
        if (s.length() >= 3) {
            return s.substr(0, 3);
        }
    }
    
    int totalPadding = 3 - s.length();
    int padLeft = totalPadding / 2;
    int padRight = totalPadding - padLeft;
    
    return string(padLeft, ' ') + s + string(padRight, ' ');
}

vector<string> UI::formatIntVector(const vector<int>& vec, int max) {
    vector<string> result(max, "   ");
    for (int i = 0; i < max && i < static_cast<int>(vec.size()); ++i) {
        result[i] = formatInt(vec[i]);
    }
    return result;
}

int convertPosToCoord(int pos) {
    if (pos >= 0 && pos <= 3) {
        return (pos * 6);
    } else if (pos == 4) {
        return 25;
    } else if (pos == -1) {
        return -5;
    } else {
        return 0;
    }
}

void UI::displayIntro(int lastLevel) {
    cout << "\n\n";
	cout << "\t" << "+-------------------------------------------------------+" << endl;
	cout << "\t" << "|                                                       |" << endl;
	cout << "\t" << "|                Human Resource Machine                 |" << endl;
	cout << "\t" << "|                                                       |" << endl;
	cout << "\t" << "+-------------------------------------------------------+" << endl;

	cout << "\t" << "  _____  " << "\t" << "  _____  " << "\t" << "  _____  " << "\t" << "  _____  " << endl;

	cout << "\t" << " [     | " << "\t" << " |     | " << "\t" << " |     | " << "\t" << " |     | " << endl;

    if (lastLevel >= 1)  {cout << "\t" << " [     | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 2)  {cout << "\t" << " [     | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 3)  {cout << "\t" << " [     | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 4)  {cout << "\t" << " [     | \n";} else {cout << "\t" << " |     | \n";};

    if (lastLevel >= 1)  {cout << "\t" << "       | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 2)  {cout << "\t" << "       | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 3)  {cout << "\t" << "       | ";} else {cout << "\t" << " |     | ";};
    if (lastLevel >= 4)  {cout << "\t" << "       | \n";} else {cout << "\t" << " |     | \n";};

	cout << "\t" << "---------" << "\t" << "---------" << "\t" << "---------" << "\t" << "---------" << endl;
	cout << "\t" << "|       |" << "\t" << "|       |" << "\t" << "|       |" << "\t" << "|       |" << endl;
	cout << "\t" << "|   1   |" << "\t" << "|   2   |" << "\t" << "|   3   |" << "\t" << "|   4   |" << endl;
	cout << "\t" << "|_______|" << "\t" << "|_______|" << "\t" << "|_______|" << "\t" << "|_______|" << endl;
}

void UI::displayUI() {
    vector<std::string> inputContent = formatIntVector(state.input, 6);
    vector<std::string> outputContent = formatIntVector(state.output, 4);
    vector<std::string> fieldContent = formatIntVector(state.field, 4);
    vector<std::string> commandContent = returnCommandContent(17);

    cout << "Level information: " << state.level;
    cout << std::string(4, '\n');

    cout << string(5, ' ') << "+---+"
         << string(5 + state.xCoord, ' ') << "+---+"
         << string(25 - state.xCoord, ' ') << "+---+"
         << string(8, ' ') << "| ===== CODE =====\n";

    cout << "IN" 
              << string(3, ' ') << "|" << inputContent[0] << "|"
              << string(5 + state.xCoord, ' ') << "|" << formatInt(state.currentBlock) << "|"
              << string(25 - state.xCoord, ' ') << "|" << outputContent[0] << "|"
              << "  OUT"
              << string(3, ' ') << "| "
              << commandContent[0] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5 + state.xCoord, ' ') << "+---+"
              << string(25 - state.xCoord, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[1] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5 + state.xCoord, ' ') << "@   @"
              << string(25 - state.xCoord, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[2] << '\n';

    cout << string(5, ' ') << "|" << inputContent[1] << "|"
              << string(5 + state.xCoord, ' ') << "-----"
              << string(25 - state.xCoord, ' ') << "|" << outputContent[1] << "|"
              << string(8, ' ') << "| "
              << commandContent[3] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5 + state.xCoord, ' ') << "|@ @|"
              << string(25 - state.xCoord, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[4] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5 + state.xCoord, ' ') << "  +  "
              << string(25 - state.xCoord, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[5] << '\n';

    cout << string(5, ' ') << "|" << inputContent[2] << "|"
              << string(5 + state.xCoord, ' ') << "/   \\"
              << string(25 - state.xCoord, ' ') << "|" << outputContent[2] << "|"
              << string(8, ' ') << "| "
              << commandContent[6] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5 + state.xCoord, ' ') << " | | "
              << string(25 - state.xCoord, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[7] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(10, ' ')
              << string(25, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[8] << '\n';

    cout << string(5, ' ') << "|" << inputContent[3] << "|"
              << string(5, ' ') << "+---+ +---+ +---+ +---+"
              << string(7, ' ') << "|" << outputContent[3] << "|"
              << string(8, ' ') << "| "
              << commandContent[9] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5, ' ') << "|" << fieldContent[0] << "| " << "|" << fieldContent[1] << "| " 
              << "|" << fieldContent[2] << "| " << "|" << fieldContent[3] << "|"
              << string(7, ' ') << "+---+"
              << string(8, ' ') << "| "
              << commandContent[10] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(5, ' ') << "+---+ +---+ +---+ +---+"
              << string(20, ' ') << "| "
              << commandContent[11] << '\n';

    cout << string(5, ' ') << "|" << inputContent[4] << "|"
              << string(5, ' ') << "  0     1     2     3  "
              << string(20, ' ') << "| "
              << commandContent[12] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(48, ' ') << "| "
              << commandContent[13] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(48, ' ') << "| "
              << commandContent[14] << '\n';

    cout << string(5, ' ') << "|" << inputContent[5] << "|"
              << string(48, ' ') << "| "
              << commandContent[15] << '\n';

    cout << string(5, ' ') << "+---+"
              << string(48, ' ') << "| "
              << commandContent[16] << "\n\n\n";
}

void UI::moveTo(int pos) {
    int targetXCoord = convertPosToCoord(pos);
    int step = (state.xCoord < targetXCoord) ? 1 : -1;

    while (state.xCoord != targetXCoord) {
        state.xCoord += step;
        UI::clearScreen();
        UI::displayUI();
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}
