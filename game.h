

#ifndef RGB_MATRIX_GAME_H
#define RGB_MATRIX_GAME_H

#include "color.h"
#include <vector>

using namespace std;

class Game {
public:
    virtual void init() = 0;

    virtual void play() = 0;

    virtual vector<vector<Color> > draw() = 0;
};

#endif //RGB_MATRIX_GAME_H
