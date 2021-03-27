

#include <chrono>

#include "life.h"

Life::Life(int board_size_x, int board_size_y, Color color) {
    this->game_state = vector(board_size_x, vector(board_size_y, false));
    this->color = color;
}

int Life::countNeighbors(int x, int y) {
    int count = 0;

    // top left
    count += (x > 0 && y > 0 && game_state[x - 1][y - 1]);

    // top
    count += (y > 0 && game_state[x][y - 1]);

    // top right
    count += (x < game_state.size() - 1 && y > 0 && game_state[x + 1][y - 1]);

    // right
    count += (x < game_state.size() - 1 && game_state[x + 1][y]);

    // bottom right
    count += (x < game_state.size() - 1 && y < game_state[x].size() - 1 && game_state[x + 1][y + 1]);

    // bottom
    count += (y < game_state[x].size() - 1 && game_state[x][y + 1]);

    // bottom left
    count += (x > 0 && y < game_state[x].size() - 1 && game_state[x - 1][y + 1]);

    // left
    count += (x > 0 && game_state[x - 1][y]);

    return count;
}

void Life::init(double init_density) {
    srand(time(0));

    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            game_state[x][y] = ((int) rand() % 100 < init_density * 100);
        }
    }

}

void Life::play() {
    vector<vector<bool> > game_state_next = game_state;

    // examine state
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            int neighbors = countNeighbors(x, y);
            if (game_state[x][y]) {
                // alive
                game_state_next[x][y] = neighbors == 2 || neighbors == 3;
            } else {
                // dead
                game_state_next[x][y] = neighbors == 3;
            }
        }
    }

    // update state
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            bool alive = game_state_next[x][y];
            game_state[x][y] = alive;
        }
    }
}

vector<vector<Color> > Life::draw() {
    vector<vector<Color> > frame(game_state.size(), vector<Color>(game_state[0].size()));
    Color off = Color(0, 0, 0);
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            frame[x][y] = game_state[x][y] ? color : off;
        }
    }
    return frame;
}
