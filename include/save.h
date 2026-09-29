#pragma once

#include <filesystem>
#include <fstream>
#include <ios>

namespace fs = std::filesystem;

struct ScreenSize
{
    int width;
    int height;
};

struct ScreenPosition
{
    int x;
    int y;
};


class Save
{
public:
    ScreenSize screen_size;
    ScreenPosition position;
    double elapsed_time;

    Save()
        : screen_size({0, 0}), position({0, 0}), elapsed_time(0.0)
    {
    };
    
    bool load_from_file(const fs::path& save_path)
    {
        if(!fs::exists(save_path))
            return false;
        std::ifstream save_file(save_path, std::ios::binary);
        if(!save_file.is_open())
            return false;
        save_file.read(reinterpret_cast<char*>(this), sizeof(Save));
        save_file.close();
        return true;
    }

    bool to_file(const fs::path& save_path)
    {
        std::ofstream save_file(save_path, std::ios::binary);
        if(!save_file.is_open())
            return false;
        save_file.write(reinterpret_cast<const char*>(this), sizeof(Save));
        save_file.close();
        return true;
    }
};
