#include "Renderer.h"

#include "Level.h"
#include "NPC.h"
#include "NpcColor.h"
#include "Player.h"

Renderer::Renderer() : console_(GetStdHandle(STD_OUTPUT_HANDLE)) {
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(console_, &info);
    originalColor_ = info.wAttributes;

    GetConsoleCursorInfo(console_, &originalCursor_);
    auto cursor = originalCursor_;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(console_, &cursor);
}

Renderer::~Renderer() {
    SetConsoleTextAttribute(console_, originalColor_);
    SetConsoleCursorInfo(console_, &originalCursor_);
    SetConsoleCursorPosition(console_, COORD{0, lastRow_});
}

void Renderer::draw(const Level& level,
                    const Player& player,
                    const std::vector<NPC>& npcs,
                    bool gameOver) {
    SetConsoleCursorPosition(console_, COORD{0, 0});

    // Стены и трава зелёные.
    SetConsoleTextAttribute(
        console_, FOREGROUND_GREEN | FOREGROUND_INTENSITY
    );

    for (const auto& row : level.rows()) {
        write(row);
        write(L"\r\n");
    }

    // Игрок жёлтый.
    SetConsoleCursorPosition(console_, COORD{player.x(), player.y()});
    SetConsoleTextAttribute(
        console_,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
    );
    write(L"●");

    // Каждый NPC хранит собственный цвет.
    for (const NPC& npc : npcs) {
        SetConsoleCursorPosition(console_, COORD{npc.x(), npc.y()});
        SetConsoleTextAttribute(console_, colorForNpc(npc.color()));
        write(L"●");
    }

    SetConsoleTextAttribute(console_, originalColor_);

    const short statusRow = static_cast<short>(level.height() + 1);
    const short helpRow = static_cast<short>(level.height() + 2);
    const short gameOverRow = static_cast<short>(level.height() + 3);
    lastRow_ = static_cast<short>(level.height() + 5);

    SetConsoleCursorPosition(console_, COORD{0, statusRow});
    write(L"Трава съедена: " + std::to_wstring(player.grassEaten()) +
          L"   Осталось: " + std::to_wstring(level.grassRemaining()) +
          L"   NPC: " + std::to_wstring(npcs.size()) +
          L"                              ");

    SetConsoleCursorPosition(console_, COORD{0, helpRow});
    write(L"Стрелки / WASD - движение    ESC - выход                  ");

    SetConsoleCursorPosition(console_, COORD{0, gameOverRow});

    if (gameOver) {
        SetConsoleTextAttribute(
            console_, FOREGROUND_RED | FOREGROUND_INTENSITY
        );
        write(L"GAME OVER! NPC поймал игрока. Нажмите любую кнопку для игры.     ");
        SetConsoleTextAttribute(console_, originalColor_);
    } else {
        write(L"                                                                    ");
    }
}

void Renderer::write(const std::wstring& text) const {
    DWORD written = 0;
    WriteConsoleW(
        console_,
        text.c_str(),
        static_cast<DWORD>(text.size()),
        &written,
        nullptr
    );
}

WORD Renderer::colorForNpc(NpcColor color) const {
    switch (color) {
        case NpcColor::Red:
            return FOREGROUND_RED | FOREGROUND_INTENSITY;
        case NpcColor::Blue:
            return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case NpcColor::Magenta:
            return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case NpcColor::Cyan:
            return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    return originalColor_;
}
