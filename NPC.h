#pragma once

#include "NpcColor.h"

class Level;
class Player;

class NPC {
public:
    NPC(short x, short y, NpcColor color);

    short x() const;
    short y() const;
    NpcColor color() const;

    bool touches(const Player& player) const;
    bool moveToward(const Level& level, const Player& player);

private:
    short x_;
    short y_;
    NpcColor color_;
};
