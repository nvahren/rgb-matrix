

#ifndef RGB_MATRIX_GAME_H
#define RGB_MATRIX_GAME_H

#include "color.h"
#include <linux/input-event-codes.h>
#include <vector>
#include <string>
#include <list>

using namespace std;

enum class Direction {
    UP, DOWN, LEFT, RIGHT
};

class Game {
public:
    virtual void init(double init_density) = 0;

    virtual void play(list<pair<char, int> > inputs) = 0;

    virtual vector<vector<Color> > draw() = 0;
};

#endif //RGB_MATRIX_GAME_H
