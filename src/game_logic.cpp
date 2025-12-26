#include "game_logic.h"
#include "utils.h"
#include <iostream>
#include <vector> // vector
#include <string> // string
#include <cctype> // tolower()
#include <algorithm> //find()
#include <fstream>
#include <thread>
#include <chrono>
// #include <limits> //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 后期发现是电脑问题，并不需要这个
using namespace std;

// Only changed function signature - added UI& ui parameter
LevelStatus playLevel(const level &lvl, UI& ui) {
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
	} else {
		cin.ignore(10000, '\n'); // consume leftover newline
		break;
	}

	vector<string> Instruction(n);
	
	// ===== read & validate instructions =====
	for (int i = 0; i < n; i++) {
		cout << i + 1 << " ";
		string cmd;
		int X = -1;
		
		cin >> cmd;
		// getline(cin >> ws, Instruction[i]); // avoid input like inbox 2; P.S 后期的 nested cin 出问题，我试试别的方法
		// cin.ignore(); (1. 12/17-2:13-ZJS) Initially, I thought I need this when I input "Input          " and it returns "Error..."; turns out it's just my typo
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

    // ===== 设置UI初始状态 =====
    // 创建UI状态对象并初始化
    UI::GameState uiState;
    uiState.input = input;
    uiState.output = {};
    uiState.field = floor;
    uiState.code = Instruction;
    uiState.currentBlock = 0;
    uiState.currentCommand = 0;
    uiState.level = lvl.currentLevel;
    ui.setGameState(uiState);
    ui.clearScreen();
    ui.displayUI();

    // ===== execute =====
	// for (string &cmd : Instruction) { (4. 12/20-7:38-ZJS) 写了半天，到 jump 时发现完蛋了 没法跳回来；忍痛把 for 换成 while loop 方可运行
	int ip = 0; // ip for instruction pointer
	while (ip < Instruction.size()) {
        // ===== 在执行指令前更新UI =====
        // 显示当前正在执行的指令
        ui.setCurrentCommand(ip);
        ui.clearScreen();
        ui.displayUI();
        
		string cmd = Instruction[ip];
		int X = -1; // 代表没 argument
		// 本来是 nested cin cmd {cin >> X} 既然没跑起来； 后来发现是压根就没把那 X 拉进去； 上网搜到这种写法； 下面我解释一下代码逻辑
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
			//cout << "inbox -> current " << currentBlock << endl;
			hasCurrentBlock = true;
            
            // ===== inbox执行后更新UI =====
            // 更新输入队列和当前方块显示
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
			output.push_back(currentBlock);
			//cout << "current -> outbox " << output.back() << endl;
			// (3. 12/17-2:51-ZJS) 本来用 LevelOne.output[i]， 忘了 i 不能通用， size 不一样; 这种 struct+vector 最高是 .begin() .back()
			hasCurrentBlock = false;
            
            // ===== outbox执行后更新UI =====
            // 更新输出队列显示
			ui.clearScreen();
			ui.moveTo(4);
            ui.setOutput(output);
		}
		else if (cmd == "add") {
			if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) { // 思路： X (start from 0) >= floor.size()  -> X号空地不存在
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}

			currentBlock += floor[X];
            
            // ===== add执行后更新UI =====
            // 更新当前方块数值显示
            ui.setCurrentBlock(currentBlock);
            ui.clearScreen();
            ui.moveTo(X);
		}
		else if (cmd == "sub") {
			if (!hasCurrentBlock || X < 0 || X >= floor.size() || floorAvailable[X]) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			currentBlock -= floor[X];
            
            // ===== sub执行后更新UI =====
            // 更新当前方块数值显示
			ui.clearScreen();
            ui.moveTo(X);
            ui.setCurrentBlock(currentBlock);
		}
		else if (cmd == "copyto") {
			if(!hasCurrentBlock || X < 0 || X >= floor.size()) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
			floor[X] = currentBlock;
			floorAvailable[X] = false;
            
            // ===== copyto执行后更新UI =====
            // 更新地板格子显示
			ui.clearScreen();
            ui.moveTo(X);
            ui.setField(floor);
		}
		else if (cmd == "copyfrom") {
			if (X < 0 || X >= floor.size() || floorAvailable[X]) {
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}

			currentBlock = floor[X];
			hasCurrentBlock = true;
            
            // ===== copyfrom执行后更新UI =====
            // 更新当前方块数值显示
			ui.clearScreen();
            ui.moveTo(X);
            ui.setCurrentBlock(currentBlock);
		}

		else if (cmd == "jump") {
			if (X < 1 || X >= Instruction.size()) { // 这里至少有另外1个 instruction 才能 loop 回去
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			}
            
            // ===== jump执行前更新UI =====
            // 高亮显示要跳转到的指令
            ui.setCurrentCommand(X-1); // X-1 because instructions are 1-indexed in input but 0-indexed in array
            ui.clearScreen();
            ui.displayUI();
            
            // 短暂暂停让玩家看到跳转目标
            this_thread::sleep_for(chrono::milliseconds(500));
            
            ip = X-1; //之后有 ip++ ，但只有不用 jump 时才会加，所以提前减之后加就扯平了
            ui.clearScreen();
			continue;
		}

		else if (cmd == "jumpifzero") {
			if (!hasCurrentBlock || X < 1 || X >= Instruction.size()) { // no currentBlock to determine jump or not
				cout << "Error on instruction " << ip + 1 << endl;
				return INSTRUCTION_ERROR;
			} 
			if (currentBlock == 0) {
                // ===== jumpifzero执行前更新UI =====
                // 高亮显示要跳转到的指令
                ui.setCurrentCommand(X-1);
                ui.clearScreen();
                ui.displayUI();
                
                // 短暂暂停让玩家看到跳转目标
                this_thread::sleep_for(chrono::milliseconds(500));
                
				ip = X-1;
                ui.clearScreen();
				continue;
			}
		}
		ip++;
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

	if (output == lvl.expectedOutput && input.empty()) {
		ofstream outputFile("lastLevel.txt");

		if (outputFile.is_open()) {
			outputFile << (lvl.currentLevel + 1);
			outputFile.close();
		} else {
			cout << "A file could not be created. Please try again later.";
		}
		return SUCCESS;
	} else {
		return FAIL;
	}
	}
}