#include "save.h"
#include "screen.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <raylib.h>

// 默认窗口大小
const ScreenSize DEFAULT_SIZE = { 300, 50};

// app运行状态存档
const char* save_path = "save";
Save save;

// 计时器已记录的时间
double elapsed_time;
bool is_running;
// 计时器显示区域
Rectangle digit_start;
char* time_str;
const size_t time_len = 16;

Texture2D digits_tx;
const int digit_tx_width = 150;
const int digit_tx_height = 190;
// 显示的数字个数(包括冒号) hh:mm:ss
const int digit_count = 8;

void HandleResize();
void HandleInput();
void TransferString(double time, char* buf, size_t buf_size);

void InitTimerScreen()
{
    namespace fs = std::filesystem;

    // 设置最小窗口大小防止显示错误
    SetWindowMinSize(DEFAULT_SIZE.width, DEFAULT_SIZE.height);
    
    if(fs::exists(save_path)) {
        save.load_from_file(save_path);
    } else {
        save.screen_size = DEFAULT_SIZE;
        int monitor = GetCurrentMonitor();
        int monitor_width = GetMonitorWidth(monitor);
        int monitor_height = GetMonitorHeight(monitor);
        save.position.x = (float)(monitor_width - save.screen_size.width)/2;
        save.position.y = (float)(monitor_height - save.screen_size.height)/2;
    }

    SetWindowSize(save.screen_size.width, save.screen_size.height);
    SetWindowPosition(save.position.x, save.position.y);
    elapsed_time = save.elapsed_time;
    HandleResize();

    is_running = false;
    time_str = new char[time_len];


    digits_tx = LoadTexture("assets/digits.png");
    SetTextureFilter(digits_tx, TEXTURE_FILTER_TRILINEAR);
}

void UpdateTimerScreen()
{
    if(IsWindowResized())
        HandleResize();

    HandleInput();

    if(is_running)
        elapsed_time += GetFrameTime();

    TransferString(elapsed_time, time_str, time_len);
}

void DrawTimerScreen()
{
    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle dest = digit_start;
        Rectangle source{0, 0, digit_tx_width, digit_tx_height};
        for(size_t i = 0; i < digit_count; i++)
        {
            int index;
            if(time_str[i] == ':')
                index = 10;
            else
                index = time_str[i] - '0';

            source.x = index * source.width;

            DrawTexturePro(digits_tx, source, dest, Vector2{}, 0.0, WHITE);

            dest.x += dest.width;
        }
        
    EndDrawing();
}

void UnloadTimerScreen()
{
    //保存窗口大小
    save.screen_size.width = GetScreenWidth();
    save.screen_size.height = GetScreenHeight();
    //保存窗口位置
    auto pos = GetWindowPosition();
    save.position.x = pos.x;
    save.position.y = pos.y;
    //保存当前计时器时间
    save.elapsed_time = elapsed_time;

    save.to_file(save_path);

    delete [] time_str;

    UnloadTexture(digits_tx);
}

std::optional<ScreenType> FinishTimerScreen()
{
    return std::nullopt;
}


void HandleResize()
{
    // 数字显示宽高比
    const double digit_wh_ratio = (double)digit_tx_width/digit_tx_height;


    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    // case 1 : 更宽
    if((double)screen_width/screen_height >= digit_wh_ratio*digit_count)
    {
        digit_start.height = screen_height;
        digit_start.width = screen_height * digit_wh_ratio;
        digit_start.y = 0;
        digit_start.x = (screen_width - (digit_start.width*digit_count))/2;
    }
    // case 2 : 更高
    else {
        digit_start.width = (float)screen_width/digit_count;
        digit_start.height = digit_start.width/digit_wh_ratio;
        digit_start.x = 0;
        digit_start.y = (screen_height - digit_start.height)/2;
    }
    

}


void HandleInput()
{
    if(IsKeyPressed(KEY_D))
    {
        auto width = GetRenderWidth();
        auto height = GetRenderHeight();
        if(IsWindowState(FLAG_WINDOW_UNDECORATED))
        {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
        }else {
            SetWindowState(FLAG_WINDOW_UNDECORATED);
        }
        SetWindowSize(width, height);
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        is_running = !is_running;
    }

    if(IsKeyPressed(KEY_R))
    {
        is_running = false;
        elapsed_time = 0.0;
    }
}

void TransferString(double time, char* buf, size_t buf_size)
{
    //目标字符串不同单位个数, 如HH:MM:SS, 3
    const int need_time_num = 3;
    int divs[need_time_num] = { 24, 60, 60 };
    int res[need_time_num] = { 0, 0, (int)time };
    for(int i=need_time_num-1; i>0; i--)
    {
        res[i-1] = res[i]/divs[i];
        if(res[i-1] == 0)
            break;
        res[i] = res[i]%divs[i];
    }
    snprintf(buf, buf_size, "%02d:%02d:%02d", res[0], res[1], res[2]);
}
