

#include "ant.h"

#include <getopt.h>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <random>

#include "led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

using namespace std;

volatile bool interrupted = false;
static void InterruptHandler(int signo) {
    cout << "Interrupted: " << signo << endl;
    interrupted = true;
}

void usage() {
    cout << "TODO" << endl;
}

int main(int argc, char **argv) {
    int num_ants = 1;
    int framerate_slowdown = 30;
    string hardware_mapping = "adafruit-hat";

    static struct option long_opts[] = {
            {"num-ants", required_argument, nullptr, 'a'},
            {"framerate-slowdown", required_argument, nullptr, 's'},
            {nullptr, 0, nullptr, 0}
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
            case 'a':
                num_ants = stoi(optarg);
                break;
            case 's':
                framerate_slowdown = stoi(optarg);
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

    vector< vector<bool> > game_state(led_matrix->width(), vector<bool>(led_matrix->height(), false));
    vector<Ant> ants;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<int> board_dist(0, game_state.size() / 2);
    uniform_real_distribution<int> dir_dist(-1, 1);

    for (int i = 0; i < num_ants; i++) {

        // put it somewhere near the middle
        int x = (int) game_state.size() / 4 + board_dist(gen);
        int y = (int) game_state[0].size() / 4 + board_dist(gen);

        // give it a random starting direction
        int vx = dir_dist(gen);
        int vy = vx == 0 ? dir_dist(gen) : 0;

        Ant ant(x, y, vx, vy);

        ants.push_back(ant);
    }

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    rgb_matrix::FrameCanvas *offscreen = led_matrix->CreateFrameCanvas();
    while (!interrupted) {

        // move ants and flip squares
        for (auto & i : ants) {
            Ant* ant = &i;
            if (ant->isAlive(game_state)) {
                int current_x = ant->getX();
                int current_y = ant->getY();
                ant->move(game_state);
                game_state[current_x][current_y] = !game_state[current_x][current_y];
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

        for (int i = 0; i < ants.size(); i++) {
            Ant ant = ants.at(i);
            cout << "Ant " << i << endl;
            cout << "  x:  " << ant.x << endl;
            cout << "  y:  " << ant.y << endl;
            cout << "  vx: " << ant.vx << endl;
            cout << "  vy: " << ant.vy << endl;
            cout << "  alive: " << ant.isAlive(game_state) << endl;
        }
#endif

        // draw offscreen then flip
        for (int x = 0; x < game_state.size(); x++) {
            for (int y = 0; y < game_state[x].size(); y++) {
                if (game_state[x][y]) {
                    offscreen->SetPixel(x, y, 128, 128, 128);
                } else {
                    offscreen->SetPixel(x, y, 0, 0, 0);
                }
            }
        }

        for (auto ant : ants) {
            if (ant.isAlive(game_state)) {
                offscreen->SetPixel(ant.getX(), ant.getY(), 192, 192, 0);
            }
        }

        offscreen = led_matrix->SwapOnVSync(offscreen, framerate_slowdown);
    }

    delete led_matrix;
}
