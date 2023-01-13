#include "Schiff.h"
#include <iostream>

Schiff::Schiff(int s) {
    size = s;
    hitpoints = 0;
}

void Schiff::setPosition(int sX, int eX, int sY, int eY) {
    startX = sX;
    endX = eX;
    startY = sY;
    endY = eY;
}

bool Schiff::contains(int x, int y) {
    if(orientation == 0) {
        if (startY == endY && endY == y) {
            if (x >= startX && x <= endX) {
                return true;
            }
        }
    } else if (orientation == 1) {
        if (startX == endX && endX == x){
            if (startY >= y && endY <= y){
                return true;
            }

        }

    }
    return false;
}

void Schiff::setOrientation(char getOrientation) {
    orientation = getOrientation;
}

void Schiff::hitted() {
    hitpoints++;
}

bool Schiff::isVersenkt() {
    if (hitpoints >= size) {
        return true;
    }
    return false;
}