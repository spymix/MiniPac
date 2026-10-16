#pragma once

#include "Input.h"
#include "Level.h"
#include "NPC.h"
#include "Player.h"
#include "Renderer.h"

#include <chrono>
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    static constexpr auto NPC_MOVE_DELAY = std::chrono::milliseconds(100);
    static constexpr auto FRAME_DELAY = std::chrono::milliseconds(16);

    Level level_;
    Player player_;
    std::vector<NPC> npcs_;
    Input input_;
    Renderer renderer_;

    bool gameOver_ = false;
    std::chrono::steady_clock::time_point lastNpcMove_;

    bool anyNpcTouchesPlayer() const;
    void updateNpcMovement();
    void waitAfterGameOver() const;
    void resetRound();
};
