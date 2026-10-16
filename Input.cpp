#include "Input.h"

#include <conio.h>

InputEvent Input::poll() const {
    if (!_kbhit()) {
        return {};
    }

    const int key = _getch();

    if (key == 27) {
        return {Direction::None, true};
    }

    if (key == 0 || key == 224) {
        switch (_getch()) {
            case 72: return {Direction::Up, false};
            case 80: return {Direction::Down, false};
            case 75: return {Direction::Left, false};
            case 77: return {Direction::Right, false};
            default: return {};
        }
    }

    switch (key) {
        case 'w': case 'W': return {Direction::Up, false};
        case 's': case 'S': return {Direction::Down, false};
        case 'a': case 'A': return {Direction::Left, false};
        case 'd': case 'D': return {Direction::Right, false};
        default: return {};
    }
}
