#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

using namespace std;

volatile bool interrupted = false;
static void InterruptHandler(int signo) {
    interrupted = true;
}

class Ant {
    public:
        int x;
        int y;
        int vx;
        int vy;

        void move(vector< vector<bool> > &game_state) {
            if (!alive) {
                return;
            }

            if (vx != 0) {
                vy = vx * game_state[x][y];
                vx = 0;
            } else {
                vx = vy * game_state[x][y];
                vy = 0;
            }

            x += vx;
            y += vy;
        }

        bool isAlive(vector< vector<bool> > &game_state) {
            if (!alive) {
                return false;
            }

            alive = x < 0 || y < 0 || x > game_state.size() - 1 || y > game_state[0].size() -1;
            return alive;
        }

    private:
        bool alive;

};

int main(int argc, char **argv) {
    int num_ants = 1;
    int framerate_slowdown = 30;
    char* hardware_mapping = "adafruit-hat";

    static struct option long_opts[] = {
        {"num-ants", required_argument, 0, 'a'},
        {"framerate-slowdown", required_argument, 0, 's'},
        {NULL, 0, NULL, 0}
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
        }
    }
    opterr = 1;

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
    vector<Ant> ants;

    srand(time(NULL));

    for (int i = 0; i < num_ants; i++) {
        Ant ant;

        // put it somewhere near the middle
        ant.x = game_state.size() / 4 + rand() % (game_state.size() / 2);
        ant.y = game_state[0].size() / 4 + rand() % (game_state[0].size() / 2);

        // give it a random starting direction
        ant.vx = rand() % 3 - 1;
        ant.vy = ant.vx == 0 ? rand() % 3 - 1 : 0;

        ants.push_back(ant);
    }

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    // #ifdef DEBUG
    // cout << "initial state" << endl;
    // for (int x = 0; x < game_state.size(); x++) {
    //     for (int y = 0; y < game_state[x].size(); y++) {
    //         cout << game_state[x][y];
    //     }
    //     cout << endl;
    // }
    // #endif

    rgb_matrix::FrameCanvas *offscreen = led_matrix->CreateFrameCanvas();
    while (!interrupted) {

        for (int i = 0; i < ants.size(); i++) {
            Ant* ant = &ants.at(i);
            int current_x = ant->x;
            int current_y = ant->y;
            ant->move(game_state);
            game_state[current_x][current_y] = !game_state[current_x][current_y];
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

        for (int x = 0; x < game_state.size(); x++) {
            for (int y = 0; y < game_state[x].size(); y++) {
                if (game_state[x][y]) {
                    offscreen->SetPixel(x, y, 128, 128, 128);
                } else {
                    offscreen->SetPixel(x, y, 0, 0, 0);
                }
            }
        }

        for (int i = 0; i < ants.size(); i++) {
            Ant ant = ants.at(i);
            if (ant.isAlive(game_state)) {
                offscreen->SetPixel(ant.x, ant.y, 0, 192, 192);
            }
        }

        // draw
        offscreen = led_matrix->SwapOnVSync(offscreen, framerate_slowdown);
    }

}
