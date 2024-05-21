

#ifndef RGB_MATRIX_NONE_H
#define RGB_MATRIX_NONE_H

#include <vector>

#include "../game.h"

class None : public Game {
 public:
  None(int board_size_x, int board_size_y);

  void init() override;

  void play() override;

  vector<vector<Color> > draw() override;

 private:
  vector<vector<Color>> frame;
};


#endif //RGB_MATRIX_NONE_H
