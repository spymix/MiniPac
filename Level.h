#pragma once

#include <string>
#include <vector>

class Level {
public:
    explicit Level(const std::string& fileName = "level.txt");

    const std::vector<std::wstring>& rows() const;

    int width() const;
    int height() const;

    short startX() const;
    short startY() const;

    bool canWalk(short x, short y) const;
    bool eatGrass(short x, short y);

    int grassRemaining() const;

private:
    static constexpr double GRASS_PROBABILITY = 0.30;

    int grassRemaining_ = 0;
    short startX_ = 0;
    short startY_ = 0;
    std::vector<std::wstring> map_;

    void findPlayerStart();
    void placeRandomGrass(short startX, short startY);
};
