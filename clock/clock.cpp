

#include "clock.h"

#include <chrono>
#include <ctime>
#include <bitset>

Clock::Clock(int board_size_x, int board_size_y) {
    this->game_state = vector(6, vector(4, false));
    this->scale = (board_size_x - 5) / 6; // leaving room for a blank line between cells
    this->board_size_x = board_size_x;
    this->board_size_y = board_size_y;
}

void Clock::init(double init_density) {
    // no-op
}

void updateGameState(vector<vector<bool> > &game_state, int digit, bitset<4> value) {
    for (int y = 0; y < 4; y++) {
        game_state[digit][y] = value[y];
    }
}

int getTens(int value) {
    return (value - (value % 10)) / 10;
}

int getOnes(int value) {
    return value % 10;
}

void Clock::play(list<pair<char, int> > inputs) {
    time_t t = time(nullptr);
    struct tm *now = localtime(&t);

    int hour = now->tm_hour;
    int minute = now->tm_min;
    int second = now->tm_sec;

    updateGameState(game_state, 0, bitset<4>(getTens(hour)));
    updateGameState(game_state, 1, bitset<4>(getOnes(hour)));
    updateGameState(game_state, 2, bitset<4>(getTens(minute)));
    updateGameState(game_state, 3, bitset<4>(getOnes(minute)));
    updateGameState(game_state, 4, bitset<4>(getTens(second)));
    updateGameState(game_state, 5, bitset<4>(getOnes(second)));
}

vector <vector<Color>> Clock::draw() {
    Color on = Color(0, 0, 128);
    Color off = Color(0, 0, 0);
    vector<vector<Color> > frame = vector(board_size_x, vector<Color>(board_size_y));
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state.size(); y++) {
            for (int xOffset = 0; xOffset < scale; xOffset++) {
                for (int yOffset = 0; yOffset < scale; yOffset++) {
                    frame[(frame.size() - 1) - (x * scale + xOffset + x + 2)][y * scale + yOffset + y + 2] = game_state[x][y] ? on : off;
                }
            }
        }
    }
    return frame;
}
