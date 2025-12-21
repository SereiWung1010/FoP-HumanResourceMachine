#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "level.h"
#include "UI.h"  // 添加UI头文件以支持界面显示

// 游戏关卡执行函数，新增UI参数用于实时显示游戏状态
LevelStatus playLevel(const level &lvl, UI& ui);

#endif // GAME_LOGIC_H