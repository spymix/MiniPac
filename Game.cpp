#include "Game.h"

#include "NpcColor.h"

#include <conio.h>
#include <thread>

Game::Game()
    : player_(level_.startX(), level_.startY()),
      npcs_{
          NPC(1, 1, NpcColor::Red),
          NPC(static_cast<short>(level_.width() - 2), 1, NpcColor::Blue),
          NPC(1, static_cast<short>(level_.height() - 2), NpcColor::Magenta),
          NPC(static_cast<short>(level_.width() - 2),
              static_cast<short>(level_.height() - 2),
              NpcColor::Cyan)
      },
      lastNpcMove_(std::chrono::steady_clock::now()) {
    player_.eatGrass(level_);
    gameOver_ = anyNpcTouchesPlayer();
}

void Game::run() {
    while (true) {
        renderer_.draw(level_, player_, npcs_, gameOver_);

        while (!gameOver_) {
            const InputEvent event = input_.poll();

            if (event.exitRequested) {
                return;
            }

            bool redrawNeeded = false;

            if (player_.move(event.direction, level_)) {
                redrawNeeded = true;
                gameOver_ = anyNpcTouchesPlayer();
            }

            const auto now = std::chrono::steady_clock::now();

            if (!gameOver_ && now - lastNpcMove_ >= NPC_MOVE_DELAY) {
                updateNpcMovement();
                lastNpcMove_ = now;
                redrawNeeded = true;
            }

            if (redrawNeeded || gameOver_) {
                renderer_.draw(level_, player_, npcs_, gameOver_);
            }

            std::this_thread::sleep_for(FRAME_DELAY);
        }

        waitAfterGameOver();
        resetRound();
    }
}

bool Game::anyNpcTouchesPlayer() const {
    for (const NPC& npc : npcs_) {
        if (npc.touches(player_)) {
            return true;
        }
    }

    return false;
}

void Game::updateNpcMovement() {
    for (NPC& npc : npcs_) {
        npc.moveToward(level_, player_);

        if (npc.touches(player_)) {
            gameOver_ = true;
            break;
        }
    }
}

void Game::waitAfterGameOver() const {
    while (_kbhit()) {
        _getch();
    }

    _getch();
}

void Game::resetRound() {
    // Level перечитывает level.txt заново. Значит, карту можно изменить
    // даже между раундами без повторной компиляции MiniPac.exe.
    level_ = Level{};
    player_ = Player(level_.startX(), level_.startY());

    npcs_ = {
        NPC(1, 1, NpcColor::Red),
        NPC(static_cast<short>(level_.width() - 2), 1, NpcColor::Blue),
        NPC(1, static_cast<short>(level_.height() - 2), NpcColor::Magenta),
        NPC(static_cast<short>(level_.width() - 2),
            static_cast<short>(level_.height() - 2),
            NpcColor::Cyan)
    };

    player_.eatGrass(level_);
    gameOver_ = anyNpcTouchesPlayer();
    lastNpcMove_ = std::chrono::steady_clock::now();
}
