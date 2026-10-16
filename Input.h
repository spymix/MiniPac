#pragma once

#include "Direction.h"

struct InputEvent {
    Direction direction = Direction::None;
    bool exitRequested = false;
};

class Input {
public:
    InputEvent poll() const;
};
