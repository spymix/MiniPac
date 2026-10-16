#include "Player.h"

#include "Level.h"

Player::Player(short x, short y) : x_(x), y_(y) {}

short Player::x() const {
    return x_;
}

short Player::y() const {
    return y_;
}

int Player::grassEaten() const {
    return grassEaten_;
}

void Player::eatGrass(Level& level) {
    if (level.eatGrass(x_, y_)) {
        ++grassEaten_;
    }
}

bool Player::move(Direction direction, Level& level) {
    short nextX = x_;
    short nextY = y_;

    switch (direction) {
        case Direction::Up:    --nextY; break;
        case Direction::Down:  ++nextY; break;
        case Direction::Left:  --nextX; break;
        case Direction::Right: ++nextX; break;
        case Direction::None:  return false;
    }

    if (!level.canWalk(nextX, nextY)) {
        return false;
    }

    x_ = nextX;
    y_ = nextY;
    eatGrass(level);
    return true;
}
