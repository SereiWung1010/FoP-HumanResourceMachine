#include "game_logic.h"
#include "level.h"
#include "UI.h"  // 添加UI头文件
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

int main() {
    UI ui;  // 创建UI对象用于显示游戏界面

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
			cout << "Please Enter a Number\n";
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

			level LevelOne;
			LevelOne.input = {1, 2};
			LevelOne.expectedOutput = {1, 2};
			LevelOne.instructionAvailable = {"inbox", "outbox"};
			LevelOne.floor = {};
			LevelOne.floorAvailable = {};
            LevelOne.currentLevel = 1;

			//cout << (LevelOne.success ? "SUCCESS" : "FAIL"); 
			LevelStatus result = playLevel(LevelOne, ui); // 传递UI对象给游戏逻辑
			if (result == SUCCESS) {
				cout << "SUCCESS" << endl;
				lastLevel = 2; //last level here 代表 not completed yet, you were here, now continue
			} else if (result == FAIL) {
				cout << "FAIL" << endl;
			}
		}

		// ========================= 4 === Level 2 ========================= //
		if (selectLevel == 2) {
			cout << "Level 2 :" << endl;

			level LevelTwo;
			LevelTwo.input = {3, 9, 5, 1, -2, -2, 9, -9};
			LevelTwo.expectedOutput = {-6, 6, 4, -4, 0, 0, 18, -18};
			LevelTwo.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			LevelTwo.floor = {0,0,0};
			LevelTwo.floorAvailable = {true, true, true};
            LevelTwo.currentLevel = 2;

			//cout << (LevelOne.success ? "SUCCESS" : "FAIL"); 
			LevelStatus result = playLevel(LevelTwo, ui); // 传递UI对象给游戏逻辑
			if (result == SUCCESS) {
				cout << "SUCCESS" << endl;
				lastLevel = 3;
			} else if (result == FAIL) {
				cout << "FAIL" << endl;
			}
		}

		// ========================= 5 === Level 3 ========================= //
		if (selectLevel == 3) {
			cout << "Level 3 :" << endl;

			level LevelThree;
			LevelThree.input = {6, 2, 7, 7, -9, 3, -3, -3};
			LevelThree.expectedOutput = {7, -3};
			LevelThree.instructionAvailable = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
			LevelThree.floor = {0,0,0};
			LevelThree.floorAvailable = {true, true, true};
            LevelThree.currentLevel = 3;

			//cout << (LevelOne.success ? "SUCCESS" : "FAIL"); 
			LevelStatus result = playLevel(LevelThree, ui); // 传递UI对象给游戏逻辑
			if (result == SUCCESS) {
				cout << "\nSUCCESS" << endl;
				lastLevel = 4;
			} else if (result == FAIL) {
				cout << "\nFAIL" << endl;
			}
		}		

		// ========================= 6 === Level 4 ========================= //
	}
	return 0;
}