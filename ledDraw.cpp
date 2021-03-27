

#include "ledDraw.h"

using namespace std;

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

LedDraw::LedDraw(int rows, int cols, const string &hardware_mapping, int framerate_slowdown, int argc, char **argv) {
    RGBMatrix::Options matrix_options;
    matrix_options.rows = rows;
    matrix_options.cols = cols;
    matrix_options.hardware_mapping = hardware_mapping.c_str();
    matrix_options.show_refresh_rate = true;
    this->framerate_slowdown = framerate_slowdown;

    rgb_matrix::RuntimeOptions runtime_defaults;
    runtime_defaults.drop_privileges = 1;

    led_matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &matrix_options, &runtime_defaults);

    if (led_matrix == nullptr) {
        PrintMatrixFlags(stderr, matrix_options, runtime_defaults);
    }

    offscreen = led_matrix->CreateFrameCanvas();

}

LedDraw::~LedDraw() {
    delete led_matrix;
}

void LedDraw::draw(const vector<vector<Color> > &frame) {
    for (int x = 0; x < frame.size(); x++) {
        for (int y = 0; y < frame[x].size(); y++) {
            offscreen->SetPixel(x, y, frame[x][y].getRed(), frame[x][y].getGreen(), frame[x][y].getBlue());
        }
    }

    offscreen = led_matrix->SwapOnVSync(offscreen, framerate_slowdown);
}
