#include <cstddef>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <system_error>


const int SCREEN_WIDTH = 350;
const int SCREEN_HEIGHT = 200;

void TransferString(double time, char* buf, size_t buf_size);
bool DrawButton(Rectangle rect, const char* text, Color base_color = Color{25, 26, 27, 128}, Color hover_color = Color{44, 45, 46, 255});
void DrawTime(Rectangle rect);

struct AppState
{
    double pause_time_count;
    double cumulative_time;
    double pause_time;
    double start_time;
    bool is_pause;
    double last_update_time;
    const double UPDATE_DELAY = 1.0;

    void clear()
    {
        pause_time_count = 0.0;
        cumulative_time = 0.0;
        pause_time = 0.0;
        start_time = 0.0;
        is_pause = true;
        last_update_time = 0.0;
    }
};

struct AppShow
{
    char* cumulative_time_str;
    Rectangle cumulative_time_rect;
    Rectangle reset_btn_rect;
    Rectangle control_btn_rect;

    void init()
    {
        cumulative_time_str = new char[16];
    }

    void unload()
    {
        delete [] cumulative_time_str;
    }
};

void HandleResize(AppShow& show);

AppState state;
AppShow show;

const float number_font_size = 100;
Font number_font;
const float btn_font_size = 30;
Font btn_font;

int main()
{
    #if defined(RELEASE_BUILD)
        ChangeDirectory(GetApplicationDirectory());
    #endif
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ctimer");
    SetTargetFPS(60);
    number_font = LoadFontEx("assets/font/Pacifico-Regular.ttf", number_font_size, NULL, 0);
    SetTextureFilter(number_font.texture, TEXTURE_FILTER_BILINEAR);
    btn_font = LoadFontEx("assets/font/PermanentMarker-Regular.ttf", btn_font_size, NULL, 0);
    SetTextureFilter(btn_font.texture, TEXTURE_FILTER_BILINEAR);


    state.clear();
    show.init();
    HandleResize(show);
    TransferString(state.cumulative_time, show.cumulative_time_str, 16);

    while(!WindowShouldClose()) {
        if(IsWindowResized())
        {
            HandleResize(show);
        }


        double cur_time = GetTime();
        if(!state.is_pause && cur_time-state.last_update_time > state.UPDATE_DELAY)
        {
            state.last_update_time = cur_time;
            state.cumulative_time = cur_time - state.start_time - state.pause_time_count;
            TransferString(state.cumulative_time, show.cumulative_time_str, 16);
        }


        BeginDrawing();
            ClearBackground(Color{0, 0, 0, 192});

            DrawTime(show.cumulative_time_rect);

            if(state.is_pause)
            {
                if(DrawButton(show.control_btn_rect, "start"))
                {
                    //第一次计时
                    if(state.start_time == 0)
                        state.start_time = cur_time;
                    else
                        state.pause_time_count += cur_time-state.pause_time;
                    state.is_pause = false;
                }
            }else {
                if(DrawButton(show.control_btn_rect, "pause"))
                {
                    state.pause_time = cur_time;
                    state.is_pause = true;
                }
            }

            if(DrawButton(show.reset_btn_rect, "reset"))
            {
                state.clear();
                TransferString(0.0, show.cumulative_time_str, 16);
            }


        EndDrawing();
    }

    UnloadFont(number_font);
    UnloadFont(btn_font);
    show.unload();
    CloseWindow();
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
    snprintf(buf, buf_size, "%02d : %02d : %02d", res[0], res[1], res[2]);
}


bool DrawButton(Rectangle rect, const char* text, Color base_color, Color hover_color)
{
    Vector2 mouse_pos = GetMousePosition();
    bool is_hover = CheckCollisionPointRec(mouse_pos, rect);
    bool is_click = is_hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color cur_color = is_hover ? hover_color : base_color;

    DrawRectangleRounded(rect, 0.4f, 0, cur_color);
    DrawRectangleRoundedLines(rect, 0.4f, 0, RAYWHITE);

    Vector2 text_size = MeasureTextEx(btn_font, text, btn_font_size, 0);
    Vector2 text_pos = {
        rect.x + (rect.width-text_size.x)/2,
        rect.y + (rect.height - text_size.y)/2
    };
    DrawTextEx(btn_font, text, text_pos, btn_font_size, 0, WHITE);

    return is_click;
}

void DrawTime(Rectangle rect)
{
    Vector2 text_size = MeasureTextEx(number_font, show.cumulative_time_str, number_font_size, 0);
    Vector2 text_pos = {
        (rect.width-text_size.x)/2 + rect.x,
        (rect.height - text_size.y)/2 + rect.y
    };

    DrawTextEx(number_font, show.cumulative_time_str, text_pos, number_font_size, 0, WHITE);
}

void HandleResize(AppShow& show)
{
    int render_width = GetRenderWidth();
    int render_height = GetRenderHeight();

    show.cumulative_time_rect = Rectangle{0, 0, (float)render_width, (float)render_height*0.6f};

    float btn_height = 50;
    float btn_width = 80;
    float btn_space = 30;
    Rectangle btn_rect = Rectangle{0, show.cumulative_time_rect.height, show.cumulative_time_rect.width, (float)render_height*0.3f};
    show.control_btn_rect = Rectangle{
        (btn_rect.width - btn_width*2 - btn_space)/2,
        btn_rect.y + (btn_rect.height - btn_height)/2,
        btn_width, 
        btn_height,
    };
    show.reset_btn_rect = Rectangle {
        show.control_btn_rect.x + btn_width + btn_space,
        show.control_btn_rect.y,
        btn_width,
        btn_height
    };


}