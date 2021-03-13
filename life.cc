#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

using namespace std;

volatile bool interrupted = false;
static void InterruptHandler(int signo) {
    interrupted = true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: ./rgb-matrix [rows] [columns]" << endl;
        exit(EXIT_FAILURE);
    }
    int rows = atoi(argv[0]);
    int cols = atoi(argv[1]);

    RGBMatrix::Options matrix_options;
    matrix_options.rows = rows;
    matrix_options.cols = cols;
    matrix_options.hardware_mapping = "adafruit-hat";
    matrix_options.show_refresh_rate = true;

    rgb_matrix::RuntimeOptions runtime_defaults;
    runtime_defaults.drop_privileges = 1;

    RGBMatrix *led_matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &matrix_options, &runtime_defaults);

    if (led_matrix == NULL) {
        PrintMatrixFlags(stderr, matrix_options, runtime_defaults);
        return 1;
    }

    int x = 0;
    int y = 0;
    while (true) {
        rgb_matrix::FrameCanvas *frame_canvas = led_matrix->CreateFrameCanvas();
        frame_canvas->SetPixel(x, y, 255, 0, 0);
        x++;

        if (x >= cols) {
            x = 0;
            y++;
        }

        if (y >= rows) {
            y = 0;
        }

        frame_canvas = led_matrix->SwapOnVSync(frame_canvas, 10);
    }

    delete led_matrix;
}
