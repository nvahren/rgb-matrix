
#include <cstdlib>
#include <ctime>

#include "ants.h"

Ants::Ants(int board_size_x, int board_size_y) {
    this->game_state = vector(board_size_x, vector(board_size_y, false));
    antColor = Color(192, 192, 0);
    squareColor = Color(128, 128, 128);
}

void Ants::init() {
    srand(time(0));
    for (int i = 0; i < 1; i++) {

        // put it somewhere near the middle
        int x = game_state.size() / 4 + (int) rand() % game_state.size() / 2;
        int y = game_state[0].size() / 4 + (int) rand() % game_state[0].size() / 2;

        auto dir = Direction(rand() % 4);

        Ant ant(x, y, dir);

        ants.push_back(ant);
    }
}

void Ants::play() {
    for (auto &i : ants) {
        Ant *ant = &i;
        if (ant->isAlive(game_state)) {
            int current_x = ant->getX();
            int current_y = ant->getY();
            ant->move(game_state);
            game_state[current_x][current_y] = !game_state[current_x][current_y];
        }
    }
}

vector<vector<Color> > Ants::draw() {
    Color off = Color(0, 0, 0);
    vector<vector<Color> > frame = vector(game_state.size(), vector<Color>(game_state[0].size()));
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            frame[x][y] = game_state[x][y] ? squareColor : off;
        }
    }

    for (auto ant : ants) {
        if (ant.isAlive(game_state)) {
            frame[ant.getX()][ant.getY()] = antColor;
        }
    }

    return frame;
}
