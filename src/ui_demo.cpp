#include "UI.h"
#include <vector>
#include <string>
#include <iostream>

int main() {
    // DEMO
    UI ui;
    
    UI::GameState state;
    state.input = {5, 10, -3, 42, 0, 99};
    state.output = {15, 20, 25};
    state.field = {1, 2, 3, 4};
    state.code = {"MOV", "ADD", "JMP"};
    state.currentBlock = -1;
    state.currentCommand = 0;
    state.level = 3;

    ui.setGameState(state);
    ui.clearScreen();
    ui.displayUI();

    ui.setCurrentBlock(5);
    ui.setInput({10, -3, 42, 0, 99});
    ui.moveTo(4);
    ui.setOutput({15, 20, 25, 5});
    ui.setCurrentBlock(-1);
    ui.moveTo(0);
    ui.setCurrentCommand(1);

    return 0;
}