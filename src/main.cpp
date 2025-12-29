#include "game_logic.h"
#include "level.h"
#include "UI.h"  // 添加UI头文件
#ifdef _WIN32
#include "windows.h"
#endif
#include <iostream>
#include <vector> // vector
#include <string> // string
#include <cctype> // tolower()
#include <algorithm> //find()
#include <fstream>

// #include <limits> //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 后期发现是电脑问题，并不需要这个
using namespace std;

int getLastLevel() {
    ifstream inputFile("lastLevel.txt");
    string line;
    string lastLevel;

    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            lastLevel = line;
        }
        inputFile.close();

    } else {
        cout << "Unable to load the completed levels correctly. Please try again later.";
    }

    try {
        return stoi(lastLevel);;
    } catch (const exception& e) {
        return 1;
    }
}

void setGameState(const level &lvl, UI& ui) {
	// ===== 设置UI初始状态 =====
    // 创建UI状态对象并初始化
    UI::GameState uiState;
	uiState.instruction = lvl.instruction;
    uiState.input = lvl.input;
    uiState.output = {};
    uiState.field = lvl.floor;
    uiState.code ={};
    uiState.currentBlock = -10000;
    uiState.currentCommand = 0;
    uiState.level = lvl.currentLevel;
    ui.setGameState(uiState);
    ui.clearScreen();
    ui.displayUI();
}

int main() {
	#ifdef _WIN32
		SetConsoleOutputCP(65001);  // set Windows console code page to UTF-8
	#endif
	UI ui;  // 创建UI对象用于显示游戏界面
	level currentLevel;

	// ========================= 1 === Intro Interface / Level Selection ========================= //

	while (true) {
        int selectLevel;
        int lastLevel = getLastLevel();
        ui.displayIntro(lastLevel);
		cout << "\n\nPlease Select Your Level : ";
		// cin >> selectLevel; //下面有 cin 了
		// ===== verify Level Completiton Status =====
		// cout << ((selectLevel > lastLevel)? "Level Locked" : 0 ); // Atp 1. I don't have else, so I put 0 as dummy exp; Atp 2. OMG, I forgot that we have else, AND IT'S THE CODE FOR THE GAME WE ARE GONNA PLAY

		// if (selectLevel < 1 || selectLevel > 4) {
		// 	cout << "Level doesn't exist" << endl;
		// 	continue;
		// } // (ZJS) 刚才误触把 gibberish 输入 level 结果把我内存干崩溃了， 笑死！ 
		// P.S 以为这样就行，但不行。(不代表没用，只是解决的是别的方法）。
		// 特意查了一下，是 cin 的问题。解决方法如下 ↓

		if (!(cin >> selectLevel)) { // (ZJS) 搞了半天你小子也在输入啊 （愤怒）
			cout << "\nPlease Enter a Number\n";
			cin.clear(); // 就是 clear
			cin.ignore(10000, '\n'); // 如果乱输入，可把前 10000 字符忽略 P.S 10000 既然在 continue 后还奏效？ 后续没法输入啊
			// cin.ignore(numeric_limits<streamsize>::max(), '\n'); // (ZJS) 直接 ignore 这次 cin，源代码从网上搬过来的。问就是本人也不知道
			// 后期发现是我电脑卡了，并非 next loop奏效
			continue;
		}

		if (selectLevel < 1 || selectLevel > 4) {
			cout << "\nLevel doesn't exist" << endl;
			continue;
		}

		if (selectLevel > lastLevel) {
			cout << "\nLevel Locked" << endl;
			continue;
		}

		// ========================= 3 === Level 1 ========================= //
		if (selectLevel == 1) {
			cout << "\nLevel 1 :" << endl;
			currentLevel.instruction = "让机器人取出输入序列上的每个积木放入输出序列中。\n\n可用指令集：inbox, outbox\n\n可用空地数：0";
			currentLevel.input = {1, 2};
			currentLevel.correctOutput = {1, 2};
			currentLevel.expectedOutput = vector<int>(
   				currentLevel.correctOutput.rbegin(),
   				currentLevel.correctOutput.rend()
			);
			currentLevel.instructionAvailable = {"inbox", "outbox"};
			currentLevel.floor = {-100000, -100000, -100000, -100000};
			currentLevel.floorAvailable = {false, false, false, false};
			currentLevel.currentLevel = 1;
			setGameState(currentLevel, ui);
		}

		// ========================= 4 === Level 2 ========================= //
		if (selectLevel == 2) {
			cout << "\nLevel 2 :" << endl;
			currentLevel.instruction = "对于输入序列中的每两个东西，先把第1个减去第2个，并把结果放在输出序列中，然后把第2个减去第1个，再把结果放在输出序列中，重复。\n\n可用指令集：inbox, outbox, copyfrom, copyto, add, sub, jump, jumpifzero\n\n可用空地数：3";
			currentLevel.input = {3, 9, 5, 1, -2, -2, 9, -9};
			currentLevel.correctOutput = {-6, 6, 4, -4, 0, 0, 18, -18};
			currentLevel.expectedOutput = vector<int>(
   				currentLevel.correctOutput.rbegin(),
   				currentLevel.correctOutput.rend()
			);
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {-10000,-10000,-10000, -100000};
			currentLevel.floorAvailable = {true, true, true, false};
			currentLevel.currentLevel = 2;
			setGameState(currentLevel, ui);
		}

		// ========================= 5 === Level 3 ========================= //
		if (selectLevel == 3) {
			cout << "\nLevel 3 :" << endl;
			currentLevel.instruction = "从输入序列中依次取2个数字，如果相等则将其中一个输出，否则扔掉。重复这个过程直到输入传送带为空。\n\n可用指令集：inbox, outbox, copyfrom, copyto, add, sub, jump, jumpifzero\n\n可用空地数：3";
			currentLevel.input = {6, 2, 7, 7, -9, 3, -3, -3};
			currentLevel.correctOutput = {7, -3};
			currentLevel.expectedOutput = vector<int>(
   				currentLevel.correctOutput.rbegin(),
   				currentLevel.correctOutput.rend()
			);
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {-10000,-10000,-10000, -100000};
			currentLevel.floorAvailable = {true, true, true};
			currentLevel.currentLevel = 3;
			setGameState(currentLevel, ui);
		}		

		// ========================= 6 === Level 4 ========================= //
		if (selectLevel == 4) {
			cout << "Level 4 :" << endl;
			currentLevel.instruction = "从第二个数开始，输出当前数与上一个数的差值，一直重复，直到输入结束。\n\n可用指令集：inbox, outbox, copyto, copyfrom, add, sub, jump, jumpifzero\n\n可用空地数：2";
			currentLevel.input = {5, 3, 8, 2, -4};
			currentLevel.correctOutput = {-2, 5, -6, -6};
			currentLevel.expectedOutput = vector<int>(
   				currentLevel.correctOutput.rbegin(),
   				currentLevel.correctOutput.rend()
			);
			currentLevel.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			currentLevel.floor = {-10000,-10000, -100000, -100000};
			currentLevel.floorAvailable = {true,true, false, false};
			currentLevel.currentLevel = 4;
			setGameState(currentLevel, ui);
		}	


		LevelStatus result = playLevel(currentLevel, ui);

		if (result == SUCCESS) {
			cout << "Success" << endl;
			lastLevel = selectLevel + 1; //last level here 代表 not completed yet, you were here, now continue
		} else if (result == FAIL) {
			cout << "Fail" << endl;
		} 
	}
	return 0;
}