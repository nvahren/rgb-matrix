

#include "ant.h"

Ant::Ant(int x, int y, Direction dir) {
    this->x = x;
    this->y = y;
    this->dir = dir;
    alive = true;
}

int Ant::getX() const {
    return x;
}

int Ant::getY() const {
    return y;
}

void Ant::move(vector<vector<bool> > &game_state) {
    if (!alive) {
        return;
    }

    switch (dir) {
        case Direction::LEFT:
            dir = game_state[x][y] ? Direction::UP : Direction::DOWN;
            dir == Direction::UP ? y++ : y--;
            break;
        case Direction::RIGHT:
            dir = game_state[x][y] ? Direction::DOWN : Direction::UP;
            dir == Direction::UP ? y++ : y--;
            break;
        case Direction::UP:
            dir = game_state[x][y] ? Direction::RIGHT : Direction::LEFT;
            dir == Direction::RIGHT ? x++ : x--;
            break;
        case Direction::DOWN:
            dir = game_state[x][y] ? Direction::LEFT : Direction::RIGHT;
            dir == Direction::RIGHT ? x++ : x--;
            break;
    }
}

bool Ant::isAlive(vector<vector<bool> > &game_state) {
    if (!alive) {
        return false;
    }

    alive = !(x < 0 || y < 0 || x > game_state.size() - 1 || y > game_state[0].size() - 1);
    return alive;
}
