

#ifndef RGB_MATRIX_ANT_H
#define RGB_MATRIX_ANT_H

#include <vector>
#include "../game.h"

using namespace std;

class Ant {
public:
    Ant(int x, int y, Direction dir);

    int getX() const;

    int getY() const;

    void move(vector<vector<bool> > &game_state);

    bool isAlive(vector<vector<bool> > &game_state);

private:
    int x{};
    int y{};
    Direction dir;
    bool alive;
};


#endif //RGB_MATRIX_ANT_H
