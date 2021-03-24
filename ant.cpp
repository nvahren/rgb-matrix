

#include "ant.h"

#ifdef DEBUG
#include <iostream>
#endif

Ant::Ant(int x, int y, int vx, int vy) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
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

    // left
    if (vx < 0) {
        vy = game_state[x][y] ? 1 : -1;
        vx = 0;

        // right
    } else if (vx > 0) {
        vy = game_state[x][y] ? -1 : 1;
        vx = 0;

        // up
    } else if (vy > 0) {
        vx = game_state[x][y] ? 1 : -1;
        vy = 0;

        // must be down
    } else {
        vx = game_state[x][y] ? -1 : 1;
        vy = 0;

    }

    x += vx;
    y += vy;
}

bool Ant::isAlive(vector<vector<bool> > &game_state) {
    if (!alive) {
        return false;
    }

    alive = !(x < 0 || y < 0 || x > game_state.size() - 1 || y > game_state[0].size() - 1);
    return alive;
}


