#include <raylib.h>
#if defined(__linux__)
    #include <stdlib.h>
#endif
#include "screen.h"

ScreenType current_screen;

void InitScreen();
void UpdateScreen();
void DrawScreen();
void UnloadScreen();
void ChangeToScreen(ScreenType next_screen);




int main()
{
    #if defined (RELEASE_BUILD)
        ChangeDirectory(GetApplicationDirectory());
    #endif

    #if defined (__linux__)
        setenv("GTK_IM_MODULE", "none", 1);
        setenv("QT_IM_MODULE", "none", 1);
        setenv("XMODIFIERS", "@im=none", 1);
    #endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
    InitWindow(100, 100, "ctimer");
    SetExitKey(KEY_Q);

    // set app icon
    Image icon = LoadImage("icon.PNG");
    ImageResize(&icon, 64, 64);
    SetWindowIcon(icon);
    UnloadImage(icon);

    SetTargetFPS(60);

    current_screen = ScreenType::Timer;
    InitScreen();
    while (!WindowShouldClose()) {
        UpdateScreen();
        DrawScreen();
    }
    UnloadScreen();

    return 0;
}

void InitScreen()
{
    switch (current_screen) 
    {
        case ScreenType::Timer:
            InitTimerScreen();
            break;
    }
}

void UpdateScreen()
{
    switch (current_screen) 
    {
        case ScreenType::Timer:
            UpdateTimerScreen();
            auto next_screen = FinishTimerScreen();
            if(next_screen.has_value())
                ChangeToScreen(next_screen.value());
            break;
    }
}

void DrawScreen()
{
    switch (current_screen) 
    {
        case ScreenType::Timer:
            DrawTimerScreen();
            break;
    }
}

void UnloadScreen()
{
    switch (current_screen) 
    {
        case ScreenType::Timer:
            UnloadTimerScreen();
            break;
    }
}

void ChangeToScreen(ScreenType next_screen)
{
    UnloadScreen();
    current_screen = next_screen;
    InitScreen();
}

