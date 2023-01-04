#include "Schiff.h"
#include <iostream>

Schiff::Schiff(int s) {
    size = s;
}

void Schiff::setPosition(int sX, int eX, int sY, int eY) {
    startX = sX;
    endX = eX;
    startY = sY;
    endY = eY;
}