

#include <getopt.h>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>

#include "game.h"
#include "life.h"

#include "led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

using namespace std;

enum games {
    UNKNOWN,
    LIFE
};

games resolveGame(const string &input) {
    if (input == "life") return LIFE;
    return UNKNOWN;
}

volatile bool interrupted = false;

static void InterruptHandler(int signo) {
    cout << "Interrupted: " << signo << endl;
    interrupted = true;
}

void usage() {
    cout << "TODO" << endl;
    exit(1);
}

int main(int argc, char **argv) {

    int red = 0;
    int green = 128;
    int blue = 0;
    int init_density = 30;
    int framerate_slowdown = 30;
    string selected_game;
    string hardware_mapping = "adafruit-hat";

    static struct option long_opts[] = {
            {"red",                required_argument, nullptr, 'r'},
            {"green",              required_argument, nullptr, 'g'},
            {"blue",               required_argument, nullptr, 'b'},
            {"init-density",       required_argument, nullptr, 'd'},
            {"framerate-slowdown", required_argument, nullptr, 's'},
            {"game",               required_argument, nullptr, 'm'},
            {nullptr, 0,                              nullptr, 0}
    };

    int opt, option_index;

    // set opterr to ignore unknown options - the LED matrix library also accepts arguments that we don't recognize here
    opterr = 0;
    while (true) {
        opt = getopt_long(argc, argv, "", long_opts, &option_index);
        if (opt == -1) {
            break;
        }

        switch (opt) {
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
            case 'm':
                selected_game = optarg;
                break;
            default:
                usage();
                break;
        }
    }
    opterr = 1;

    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = hardware_mapping.c_str();
    matrix_options.show_refresh_rate = true;

    rgb_matrix::RuntimeOptions runtime_defaults;
    runtime_defaults.drop_privileges = 1;

    RGBMatrix *led_matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &matrix_options, &runtime_defaults);

    if (led_matrix == nullptr) {
        PrintMatrixFlags(stderr, matrix_options, runtime_defaults);
        return 1;
    }

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    Game *game;
    switch (resolveGame(selected_game)) {
        case LIFE:
            game = new Life(led_matrix->width(), led_matrix->height(), Color(red, green, blue));
            break;
        default:
            game = nullptr;
            usage();
            break;
    }

    game->init(init_density);

    rgb_matrix::FrameCanvas *offscreen = led_matrix->CreateFrameCanvas();
    while (!interrupted) {
        game->play();
        game->draw(offscreen);
        offscreen = led_matrix->SwapOnVSync(offscreen, framerate_slowdown);
    }

    delete led_matrix;
    return 0;
}
