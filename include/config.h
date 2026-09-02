#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace Config {
    std::unordered_map<std::string, std::string> LoadConfig(const std::filesystem::path& config_path);
}