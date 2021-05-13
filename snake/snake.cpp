

#include "snake.h"

Snake::Snake(int board_size_x, int board_size_y, Color color) {
    this->game_state = vector(board_size_x, vector(board_size_y, false));
}

void Snake::init(double init_density) {
// put it somewhere near the middle
    x = game_state.size() / 4 + (int) rand() % game_state.size() / 2;
    y = game_state[0].size() / 4 + (int) rand() % game_state[0].size() / 2;

    dir = Direction(rand() % 4);
}

void Snake::play(list<pair<char, int> > inputs) {
            list<pair<char, int> >::iterator it;
        for (it = inputs.begin(); it != inputs.end(); it++) {
            if (it->first == 'D') {
                switch (it->second) {
                    case KEY_UP:
                        dir = Direction::UP;
                        break;
                    case KEY_DOWN:
                        dir = Direction::DOWN;
                        break;
                    case KEY_LEFT:
                        dir = Direction::LEFT;
                        break;
                    case KEY_RIGHT:
                        dir = Direction::RIGHT;
                        break;
                }
            }
        }

        switch (dir) {
            case Direction::UP:
                y++;
                break;
            case Direction::DOWN:
                y--;
                break;
            case Direction::LEFT:
                x--;
                break;
            case Direction::RIGHT:
                x++;
                break;
        }
}

vector<vector<Color> > Snake::draw() {
    Color off = Color(0, 0, 0);
    vector<vector<Color> > frame = vector(game_state.size(), vector<Color>(game_state[0].size()));
    for (int ix = 0; ix < game_state.size(); ix++) {
        for (int iy = 0; iy < game_state[ix].size(); iy++) {
            frame[ix][iy] = off;
        }
    }

    frame[x][y] = Color(255, 0, 0);

    return frame;
}
