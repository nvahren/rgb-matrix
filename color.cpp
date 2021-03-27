

#include "color.h"

Color::Color(int red, int green, int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

int Color::getRed() const { return red; }

int Color::getGreen() const { return green; }

int Color::getBlue() const { return blue; }

bool operator==(Color color, Color other) {
    if (color.getRed() != other.getRed() ||
        color.getGreen() != other.getGreen() ||
        color.getBlue() != other.getBlue()) {
        return false;
    } else {
        return true;
    }
}
