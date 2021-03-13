#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <getopt.h>

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

void usage() {

}

int main(int argc, char **argv) {

    int red = 0;
    int green = 255;
    int blue = 0;
    int init_density = 30;
    int framerate_slowdown = 30;
    char* hardware_mapping = "adafruit-hat";

    static struct option long_opts[] = {
        {"red", required_argument, 0, 'r'},
        {"green", required_argument, 0, 'g'},
        {"blue", required_argument, 0, 'b'},
        {"init-density", required_argument, 0, 'd'},
        {"framerate-slowdown", required_argument, 0, 's'},
        {NULL, 0, NULL, 0}
    };

    int c, option_index;
    while (true) {
        c = getopt_long(argc, argv, "", long_opts, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'r':
                red = stoi(optarg);
                break;
            case 'g':
                green = stoi(optarg);
                break;
            case 'b':
                blue = stoi(optarg);
                break;
             case 'd':
                init_density = stoi(optarg);
                break;
             case 's':
                framerate_slowdown = stoi(optarg);
                break;
        }
    }

    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = hardware_mapping;
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

    // create initial state
    srand(time(NULL));
    for (int x = 0; x < game_state.size(); x++) {
        for (int y = 0; y < game_state[x].size(); y++) {
            game_state[x][y] = rand() % 100 < init_density;
        }
    }

    #ifdef DEBUG
    cout << "initial state" << endl;
    for (int x = 0; x < game_state.size(); x++) {
            for (int y = 0; y < game_state[x].size(); y++) {
                cout << game_state[x][y];
            }
            cout << endl;
    }
    #endif

    rgb_matrix::FrameCanvas *offscreen = led_matrix->CreateFrameCanvas();
    while (!interrupted) {

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
                bool alive = game_state_next[x][y];
                game_state[x][y] = alive;
                if (alive) {
                    offscreen->SetPixel(x, y, red, green, blue);
                } else {
                    offscreen->SetPixel(x, y, 0, 0, 0);
                }
            }
        }

        #ifdef DEBUG
        cout << endl << "current state" << endl;
        for (int x = 0; x < game_state.size(); x++) {
                for (int y = 0; y < game_state[x].size(); y++) {
                    cout << game_state[x][y];
                }
                cout << endl;
        }
        #endif

        // draw
        offscreen = led_matrix->SwapOnVSync(offscreen, framerate_slowdown);
    }

    delete led_matrix;
}
