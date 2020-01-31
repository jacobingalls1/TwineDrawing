#include "point.h"
#include <cstdlib>
#include <cmath>
using namespace std;

point::point(int rad, int centX, int centY, int xBound, int yBound, bool random) {
    this->rad = rad;
    x = centX;
    y = centY;
    cX = centX;
    cY = centY;
    this->xBound = xBound;
    this->yBound = yBound;
    if (rand) {
        x = (rand() % centX) + centX / 2;
        y = (rand() % centY) + centY / 2;
        rectify();
    }
}

bool point::within(int max) {
    return 0 <= x and x <= max and 0 <= y and y <= max;
}

void point::reset(int min, int max) {
    if (min == max) {
        x = (rand() % (int) (2 * rad)) - rad + cX;
        y = (rand() % (int) (2 * rad)) - rad + cY;
    } else {
        x = rand() % (max - 2 * min) + 2 * min;
        y = rand() % (max - 2 * min) + 2 * min;
    }

//    if (rand() % 2 == 0) {
//        y = (sqrt(rad * rad - ((cX - x) * (cX - x))) + cY);
//    } else {
//        y = -(sqrt(rad * rad - ((cX - x) * (cX - x))) + cY);
//    }
}

void point::move(int centX, int centY) {
    x += ((pow((x - centX), 3) / abs(x - centX)) / 10000);
    y += ((pow((y - centY), 3) / abs(y - centY)) / 10000);
}

void point::rectify() {
    if (x < 0) {
        x = 1;
    }
    if (x > xBound) {
        x = xBound;
    }
    if (y < 0) {
        y = 1;
    }
    if (y > yBound) {
        y = yBound;
    }
}
