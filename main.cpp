

#include <getopt.h>
#include <csignal>
#include <iostream>
#include <map>
#include "unistd.h"

#include "game.h"
#include "keyboardInput.h"
#include "life/life.h"
#include "ants/ants.h"
#include "clock/clock.h"
#include "snake/snake.h"

#ifndef MACOS
#include "ledDraw.h"
#endif

using namespace std;

enum games {
    UNKNOWN,
    LIFE,
    CLOCK,
    ANTS,
    SNAKE
};

games resolveGame(const string &input) {
    if (input == "life") return LIFE;
    if (input == "ants") return ANTS;
    if (input == "clock") return CLOCK;
    if (input == "snake") return SNAKE;
    return UNKNOWN;
}

volatile bool interrupted = false;

static void InterruptHandler(int signo) {
    cout << "Interrupted: " << signo << endl;
    interrupted = true;
}

void usage() {
    cout << "Usage: rgb_matrix --game=[game]" << endl;
    cout << "Games:" << endl;
    cout << "  life" << endl;
    cout << "  ants" << endl;
    cout << "  clock" << endl;
    cout << "  snake" << endl;
    cout << "Options:" << endl;
    cout << "  --red" << endl;
    cout << "  --green" << endl;
    cout << "  --blue" << endl;
    cout << "  --rows" << endl;
    cout << "  --cols" << endl;
    cout << "  --init-density" << endl;
    cout << "  --framerate-slowdown" << endl;
    exit(1);
}

void drawToTerminal(const vector<vector<Color> > &frame, int framerate_slowdown) {
    cout << endl << endl;
    Color off = Color(0, 0, 0);
    for (int x = 0; x < frame.size(); x++) {
        for (int y = 0; y < frame[x].size(); y++) {
            if (frame[x][y] == off) {
                cout << " ";
            } else {
                cout << "X";
            }
        }
        cout << endl;
    }
    usleep(framerate_slowdown * 15000);
}

int main(int argc, char **argv) {

    int red = 0;
    int green = 128;
    int blue = 0;
    int rows = 64;
    int cols = 64;
    double init_density = 0.3;
    int framerate_slowdown = 30;
    string selected_game;
    string hardware_mapping = "adafruit-hat";

    static struct option long_opts[] = {
            {"red",                required_argument, nullptr, 'r'},
            {"green",              required_argument, nullptr, 'g'},
            {"blue",               required_argument, nullptr, 'b'},
            {"rows",               required_argument, nullptr, 'x'},
            {"cols",               required_argument, nullptr, 'y'},
            {"init-density",       required_argument, nullptr, 'd'},
            {"framerate-slowdown", required_argument, nullptr, 's'},
            {"game",               required_argument, nullptr, 'm'},
            {nullptr, 0,                              nullptr, 0}
    };

    int opt, option_index;

    // TODO remove from argv when we consume arguments here?
    // set opterr to ignore unknown options - the LED matrix library also accepts arguments that we don't recognize here
    opterr = 0;

    initializeKeyboardMonitor();

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
            case 'x':
                rows = stoi(optarg);
                break;
            case 'y':
                cols = stoi(optarg);
                break;
            case 'd':
                init_density = stod(optarg);
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

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    Game *game;
    switch (resolveGame(selected_game)) {
        case LIFE:
            game = new Life(cols, rows, Color(red, green, blue));
            break;
        case ANTS:
            game = new Ants(cols, rows);
            break;
        case CLOCK:
            game = new Clock(cols, rows);
            break;
        case SNAKE:
            game = new Snake(cols, rows, Color(red, green, blue));
            break;
        default:
            game = nullptr;
            usage();
            break;
    }

    game->init(init_density);

#ifndef MACOS
    LedDraw led = LedDraw(rows, cols, hardware_mapping, framerate_slowdown, argc, argv);
#endif

    while (!interrupted) {
        game->play(getKeyboardInput());
        vector<vector<Color> > frame = game->draw();
#ifdef MACOS
        drawToTerminal(frame, framerate_slowdown);
#else
        led.draw(frame);
#endif
    }
    return 0;
}
