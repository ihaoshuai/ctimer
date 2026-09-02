#include "config.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

void debug(const std::string& msg)
{
    std::cout << "[DEBUG][CONFIG]" << msg << std::endl;
}

std::ifstream OpenFile(const fs::path& path);
inline void ltrim(std::string& s);
inline void rtrim(std::string& s);
inline void trim(std::string& s);

namespace Config 
{
    const char KV_SEPARATOR = ':';

    bool IsCommentLine(const std::string& line)
    {
        return line[0]=='#' || (line[0]=='/' && line[1]=='/');
    }

    std::unordered_map<std::string, std::string> LoadConfig(const std::filesystem::path& config_path)
    {
        std::ifstream config_file = OpenFile(config_path);
        
        std::string line;
        unsigned int line_num = 0;
        std::unordered_map<std::string, std::string> configs;

        while(std::getline(config_file, line))
        {
            line_num++;
            if(line.empty() || IsCommentLine(line))
                continue;

            //配置行处理
            trim(line);
            debug(line);

            size_t separator_pos = line.find_first_of(KV_SEPARATOR);
            if(separator_pos == std::string::npos)
            {
                throw std::runtime_error("[ERROR] invalid format config file in the " + std::to_string(line_num) + " line : " + config_path.string());
            }

            auto k = line.substr(0, separator_pos);
            rtrim(k);
            auto v = line.substr(separator_pos+1, line.size()-separator_pos-1);
            ltrim(v);

            debug("k: " + k + ", v: " + v + "[endflag]");

            configs[k] = v;
        }
        return configs;
    }    
}



std::ifstream OpenFile(const fs::path& path)
{
    if(!fs::exists(path))
        throw std::runtime_error("[ERROR] file path is not existed: " + path.string());

    std::ifstream file(path);
    if(!file.is_open())
        throw std::runtime_error("[ERROR] failed to open file: " + path.string());

    return file;
}

inline void ltrim(std::string& s)
{
    s.erase(
        s.begin(), 
        std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        })
    );
}

inline void rtrim(std::string& s)
{
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(),
        s.end()
    );
}

inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}
