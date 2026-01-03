#ifndef UI_H
#define UI_H

#include <vector>
#include <string>

class UI {
public:

    struct GameState {
        std::vector<int> input;
        std::vector<int> output;
        std::vector<int> field;
        std::vector<std::string> code;
        std::string instruction;
        int currentBlock;
        int currentCommand;
        int level;
        int xCoord = 0;
    };

    UI();
    void setGameState(const GameState& newState);
    void clearScreen();
    void displayUI();
    void setInput(const std::vector<int>& input);
    void setOutput(const std::vector<int>& output);
    void setField(const std::vector<int>& field);
    void setCode(const std::vector<std::string>& code);
    void setCurrentBlock(int currentBlock);
    void setCurrentCommand(int currentCommand);
    void setLevel(int level);
    void moveTo(int pos);
    void displayIntro(int lastLevel);
    
private:
    GameState state;
    std::string formatInt(int value);
    std::vector<std::string> formatIntVector(const std::vector<int>& vec, int max);
    std::vector<std::string> returnCommandContent(int max);
};

#endif