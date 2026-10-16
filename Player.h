#pragma once

#include "Direction.h"

class Level;

class Player {
public:
    Player(short x, short y);

    short x() const;
    short y() const;
    int grassEaten() const;

    void eatGrass(Level& level);
    bool move(Direction direction, Level& level);

private:
    short x_;
    short y_;
    int grassEaten_ = 0;
};
