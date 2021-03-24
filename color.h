

#ifndef RGB_MATRIX_COLOR_H
#define RGB_MATRIX_COLOR_H


class Color {
public:
    Color();

    Color(int red, int green, int blue);

    int getRed() const;

    int getGreen() const;

    int getBlue() const;

private:
    int red;
    int green;
    int blue;
};


#endif //RGB_MATRIX_COLOR_H
