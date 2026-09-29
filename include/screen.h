#pragma once

#include <optional>

enum class ScreenType
{
    Timer
};

extern ScreenType current_screen;


void InitTimerScreen();
void UpdateTimerScreen();
void DrawTimerScreen();
void UnloadTimerScreen();
std::optional<ScreenType> FinishTimerScreen();

