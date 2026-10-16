#include "LevelLoader.h"

#include <codecvt>
#include <fstream>
#include <locale>
#include <stdexcept>

std::vector<std::wstring> LevelLoader::load(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Не удалось открыть файл уровня: " + fileName);
    }

    std::vector<std::wstring> rows;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        // Windows-текстовые файлы заканчивают строку CRLF. getline убирает LF,
        // поэтому отдельно удаляем оставшийся CR.
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // UTF-8 BOM может находиться в начале первого ряда.
        if (firstLine && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            line.erase(0, 3);
        }
        firstLine = false;

        rows.push_back(converter.from_bytes(line));
    }

    if (rows.empty()) {
        throw std::runtime_error("Файл уровня пуст: " + fileName);
    }

    const std::size_t width = rows.front().size();

    if (width == 0) {
        throw std::runtime_error("Первая строка уровня пустая: " + fileName);
    }

    for (std::size_t y = 0; y < rows.size(); ++y) {
        if (rows[y].size() != width) {
            throw std::runtime_error(
                "Ошибка level.txt: строка " + std::to_string(y + 1) +
                " имеет ширину " + std::to_string(rows[y].size()) +
                ", ожидалось " + std::to_string(width)
            );
        }
    }

    return rows;
}
