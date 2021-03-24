

#ifndef RGB_MATRIX_LIFE_H
#define RGB_MATRIX_LIFE_H

#include <vector>

#include "game.h"
#include "color.h"

using namespace std;

class Life : public Game {
public:
    Life(int board_size_x, int board_size_y, Color color);

    void init(int init_density) override;

    void play() override;

    void draw(rgb_matrix::FrameCanvas *offscreen) override;

private:
    vector<vector<bool>> game_state;
    Color color;

    int countNeighbors(int x, int y);
};


#endif //RGB_MATRIX_LIFE_H
