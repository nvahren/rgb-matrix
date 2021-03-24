

#include "color.h"

Color::Color() {
    red = 128;
    green = 128;
    blue = 128;
}

Color::Color(int red, int green, int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

int Color::getRed() const { return red; }

int Color::getGreen() const { return green; }

int Color::getBlue() const { return blue; }


