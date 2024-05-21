

#include <csignal>
#include <iostream>
#include <pigpio.h>

#include "game.h"
#include "none/none.h"
#include "life/life.h"
#include "ants/ants.h"
#include "clock/clock.h"

#include "ledDraw.h"

using namespace std;

volatile bool interrupted = false;

static void InterruptHandler(int signo) {
    cout << "Interrupted: " << signo << endl;
    interrupted = true;
}

int main(int argc, char **argv) {

    int red = 0;
    int green = 128;
    int blue = 0;
    int rows = 64;
    int cols = 64;
    double init_density = 0.3;
    int framerate_slowdown = 30;
    string hardware_mapping = "adafruit-hat";

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    std::vector<Game *> games;
    games.push_back(new None(cols, rows));
    games.push_back(new Life(cols, rows, Color(red, green, blue)));
    games.push_back(new Ants(cols, rows));
    games.push_back(new Clock(cols, rows));

    std::vector<Game *>::iterator currentGame = games.begin();
    ++currentGame;
    ++currentGame;
    ++currentGame;
    Game* game = *currentGame;

    if (gpioInitialise() < 0) {
        cout << "Failed to initialize GPIO library" << endl;
    }

    gpioSetMode(19, PI_INPUT);
    LedDraw led =
        LedDraw(rows, cols, hardware_mapping, framerate_slowdown, argc, argv);

    int reset = 0;
    int buttonHeld = 0;
    while (!interrupted) {
        game->init(init_density);
        reset = 0;

        while (!reset) {
            game->play();
            vector<vector<Color> > frame = game->draw();

            led.draw(frame);

            if (gpioRead(19) > 0) {
                buttonHeld++;
            } else {
                if (buttonHeld > 5) {
                    if(currentGame != games.end()) {
                        ++currentGame;
                    }
                    if(currentGame == games.end()){
                        currentGame = games.begin();
                    }
                    game = *currentGame;
                    reset = 1;
                } else if (buttonHeld > 0) {
                    reset = 1;
                }
                buttonHeld = 0;
            }
        }
    }
    return 0;
}
