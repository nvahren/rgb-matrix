

#ifndef RGB_MATRIX_CLOCK_H
#define RGB_MATRIX_CLOCK_H

#include <vector>

#include "../game.h"

using namespace std;

class Clock : public Game {
public:
    Clock(int board_size_x, int board_size_y);

    void init() override;

    void play() override;

    vector<vector<Color> > draw() override;

private:
    vector<vector<bool>> game_state;
    int scale;
    int board_size_x;
    int board_size_y;
};


#endif //RGB_MATRIX_CLOCK_H
