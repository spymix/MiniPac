#pragma once

#include <string>
#include <vector>

class LevelLoader {
public:
    static std::vector<std::wstring> load(const std::string& fileName);
};
