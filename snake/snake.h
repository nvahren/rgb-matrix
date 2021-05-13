

#ifndef RGB_MATRIX_SNAKE_H
#define RGB_MATRIX_SNAKE_H

#include "../game.h"

using namespace std;

class Snake : public Game {
public:
    Snake(int board_size_x, int board_size_y, Color color);

    void init(double init_density) override;

    void play(list<pair<char, int> > inputs) override;

    vector<vector<Color> > draw() override;

private:
    vector<vector<bool>> game_state;
    int x{};
    int y{};
    Direction dir;
    Color color;
};


#endif //RGB_MATRIX_SNAKE_H
