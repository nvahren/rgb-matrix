

#ifndef RGB_MATRIX_GAME_H
#define RGB_MATRIX_GAME_H

#include "led-matrix.h"

using namespace std;

class Game {
public:
    virtual void init(int init_density) = 0;

    virtual void play() = 0;

    virtual void draw(rgb_matrix::FrameCanvas *offscreen) = 0;

private:
    vector<vector<bool> > game_state;
};

#endif //RGB_MATRIX_GAME_H
