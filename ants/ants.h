

#ifndef RGB_MATRIX_ANTS_H
#define RGB_MATRIX_ANTS_H

#include <vector>

#include "ant.h"
#include "../game.h"
#include "../color.h"

class Ants : public Game {
public:
    Ants(int board_size_x, int board_size_y);

    void init() override;

    void play() override;

    vector<vector<Color> > draw() override;

private:
    vector<vector<bool>> game_state;
    vector<Ant> ants;
    Color antColor;
    Color squareColor;
};


#endif //RGB_MATRIX_ANTS_H
