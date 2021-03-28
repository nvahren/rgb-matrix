

#ifndef RGB_MATRIX_LEDDRAW_H
#define RGB_MATRIX_LEDDRAW_H

#include <string>

#include "led-matrix.h"
#include "color.h"

using namespace std;

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

class LedDraw {
public:
    LedDraw(int rows, int cols, const string &hardware_mapping, int framerate_slowdown, int argc, char **argv);

    ~LedDraw();

    void draw(const vector<vector<Color> > &frame);

private:
    RGBMatrix *led_matrix;
    rgb_matrix::FrameCanvas *offscreen;
    int framerate_slowdown;
};


#endif //RGB_MATRIX_LEDDRAW_H
