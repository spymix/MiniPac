#pragma once

#include <windows.h>
#include <string>
#include <vector>

class Level;
class Player;
class NPC;

enum class NpcColor;

class Renderer {
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void draw(const Level& level,
              const Player& player,
              const std::vector<NPC>& npcs,
              bool gameOver);

private:
    HANDLE console_;
    WORD originalColor_{};
    CONSOLE_CURSOR_INFO originalCursor_{};
    short lastRow_ = 35;

    void write(const std::wstring& text) const;
    WORD colorForNpc(NpcColor color) const;
};
