#include "NPC.h"

#include "Level.h"
#include "Player.h"

#include <queue>
#include <utility>
#include <vector>

NPC::NPC(short x, short y, NpcColor color)
    : x_(x), y_(y), color_(color) {}

short NPC::x() const {
    return x_;
}

short NPC::y() const {
    return y_;
}

NpcColor NPC::color() const {
    return color_;
}

bool NPC::touches(const Player& player) const {
    return x_ == player.x() && y_ == player.y();
}

bool NPC::moveToward(const Level& level, const Player& player) {
    const int width = level.width();
    const int height = level.height();

    std::vector<std::vector<int>> distance(
        static_cast<std::size_t>(height),
        std::vector<int>(static_cast<std::size_t>(width), -1)
    );

    std::queue<std::pair<short, short>> cells;
    cells.push({player.x(), player.y()});
    distance[player.y()][player.x()] = 0;

    const short dx[] = {0, 0, -1, 1};
    const short dy[] = {-1, 1, 0, 0};

    while (!cells.empty()) {
        const auto [x, y] = cells.front();
        cells.pop();

        for (int i = 0; i < 4; ++i) {
            const short nx = static_cast<short>(x + dx[i]);
            const short ny = static_cast<short>(y + dy[i]);

            if (!level.canWalk(nx, ny) || distance[ny][nx] != -1) {
                continue;
            }

            distance[ny][nx] = distance[y][x] + 1;
            cells.push({nx, ny});
        }
    }

    int bestDistance = distance[y_][x_];
    short bestX = x_;
    short bestY = y_;

    if (bestDistance < 0) {
        return false;
    }

    for (int i = 0; i < 4; ++i) {
        const short nx = static_cast<short>(x_ + dx[i]);
        const short ny = static_cast<short>(y_ + dy[i]);

        if (!level.canWalk(nx, ny)) {
            continue;
        }

        const int candidateDistance = distance[ny][nx];

        if (candidateDistance >= 0 && candidateDistance < bestDistance) {
            bestDistance = candidateDistance;
            bestX = nx;
            bestY = ny;
        }
    }

    if (bestX == x_ && bestY == y_) {
        return false;
    }

    x_ = bestX;
    y_ = bestY;
    return true;
}
