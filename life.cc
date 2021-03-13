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

int countNeighbors(vector< vector<bool> > &game_state, int x, int y) {
    int count = 0;

    // top left
    if (x > 0 && y > 0 && game_state[x-1][y-1]) {
        count++;
    }

    // top
    if (y > 0 && game_state[x][y-1]) {
        count++;
    }

    // top right
    if (x < game_state.size() - 1 && y > 0 && game_state[x+1][y-1]) {
        count++;
    }

    // right
    if (x < game_state.size() - 1 && game_state[x+1][y]) {
        count++;
    }

    // bottom right
    if (x < game_state.size() - 1 && y < game_state[x].size() - 1 && game_state[x+1][y+1]) {
        count++;
    }

    // bottom
    if (y < game_state[x].size() - 1 && game_state[x][y+1]) {
        count++;
    }

    // bottom left
    if (x > 0 && y < game_state[x].size() - 1 && game_state[x-1][y+1]) {
        count++;
    }

    // left
    if (x > 0 && game_state[x-1][y]) {
        count++;
    }

    return count;
}

int main(int argc, char **argv) {

    // TODO configurable
    int init_density = 30;

    int red = 0;
    int green = 255;
    int blue = 0;

    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = "adafruit-hat";
    matrix_options.show_refresh_rate = true;

    rgb_matrix::RuntimeOptions runtime_defaults;
    runtime_defaults.drop_privileges = 1;

    RGBMatrix *led_matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &matrix_options, &runtime_defaults);

    if (led_matrix == NULL) {
        PrintMatrixFlags(stderr, matrix_options, runtime_defaults);
        return 1;
    }

    vector< vector<bool> > game_state(led_matrix->width(), vector<bool>(led_matrix->height(), 0));
    vector< vector<bool> > game_state_next(led_matrix->width(), vector<bool>(led_matrix->height(), 0));

    srand(time(NULL));

    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            game_state[x][y] = rand() % 100 < init_density;
        }
    }

    rgb_matrix::FrameCanvas *offscreen = led_matrix->CreateFrameCanvas();
    while (true) {

        // examine state
        for (int x = 0; x < game_state.size(); x++) {
            for (int y = 0; y < game_state[x].size(); y++) {
                int neighbors = countNeighbors(game_state, x, y);
                if (game_state[x][y]) {
                    // alive
                    game_state_next[x][y] = neighbors == 2 || neighbors == 3;
                } else {
                    // dead
                    game_state_next[x][y] = neighbors == 3;
                }
            }
        }

        // update state
        for (int x = 0; x < game_state.size(); x++) {
            for (int y = 0; y < game_state[x].size(); y++) {
                game_state[x][y] = game_state_next[x][y];
                offscreen->SetPixel(x, y, red, green, blue);
            }
        }

        // draw
        offscreen = led_matrix->SwapOnVSync(offscreen, 20);
    }

    delete led_matrix;
}
