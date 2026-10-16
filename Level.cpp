#include "Level.h"

#include "LevelLoader.h"

#include <cstddef>
#include <queue>
#include <random>
#include <stdexcept>
#include <utility>

Level::Level(const std::string& fileName)
    : map_(LevelLoader::load(fileName)) {
    findPlayerStart();
    placeRandomGrass(startX_, startY_);
}

const std::vector<std::wstring>& Level::rows() const {
    return map_;
}

int Level::width() const {
    return static_cast<int>(map_.front().size());
}

int Level::height() const {
    return static_cast<int>(map_.size());
}

short Level::startX() const {
    return startX_;
}

short Level::startY() const {
    return startY_;
}

bool Level::canWalk(short x, short y) const {
    if (x < 0 || y < 0) {
        return false;
    }

    const auto row = static_cast<std::size_t>(y);
    const auto column = static_cast<std::size_t>(x);

    return row < map_.size() &&
           column < map_[row].size() &&
           (map_[row][column] == L' ' || map_[row][column] == L'·');
}

bool Level::eatGrass(short x, short y) {
    if (!canWalk(x, y)) {
        return false;
    }

    wchar_t& cell = map_[static_cast<std::size_t>(y)]
                        [static_cast<std::size_t>(x)];

    if (cell != L'·') {
        return false;
    }

    cell = L' ';
    --grassRemaining_;
    return true;
}

int Level::grassRemaining() const {
    return grassRemaining_;
}

void Level::findPlayerStart() {
    bool found = false;

    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            if (map_[y][x] != L'P') {
                continue;
            }

            if (found) {
                throw std::runtime_error(
                    "В level.txt должно быть только одно место старта P"
                );
            }

            startX_ = static_cast<short>(x);
            startY_ = static_cast<short>(y);
            map_[y][x] = L' ';
            found = true;
        }
    }

    if (!found) {
        throw std::runtime_error(
            "В level.txt не найден старт игрока. Добавьте символ P"
        );
    }
}

void Level::placeRandomGrass(short startX, short startY) {
    // Трава появляется только в той связной части карты,
    // до которой игрок действительно может добраться.
    std::vector<std::vector<bool>> reachable(
        map_.size(),
        std::vector<bool>(map_.front().size(), false)
    );

    std::queue<std::pair<int, int>> cells;
    cells.push({startX, startY});
    reachable[startY][startX] = true;

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    while (!cells.empty()) {
        const auto [x, y] = cells.front();
        cells.pop();

        for (int i = 0; i < 4; ++i) {
            const int nx = x + dx[i];
            const int ny = y + dy[i];

            if (nx < 0 || ny < 0 || ny >= height() || nx >= width()) {
                continue;
            }

            if (reachable[ny][nx] || map_[ny][nx] != L' ') {
                continue;
            }

            reachable[ny][nx] = true;
            cells.push({nx, ny});
        }
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::bernoulli_distribution grassHere(GRASS_PROBABILITY);

    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            if (reachable[y][x] && grassHere(generator)) {
                map_[y][x] = L'·';
                ++grassRemaining_;
            }
        }
    }
}
