

#include "none.h"

#include <chrono>
#include <thread>

None::None(int board_size_x, int board_size_y) {
    Color off = Color(0, 0, 0);
    frame = vector<vector
                       <Color >>(board_size_x,
                                 vector<Color>(board_size_y, off));
}

void None::init() {
    // no-op
}

void None::play() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

vector <vector<Color>> None::draw() {
    return frame;
}
