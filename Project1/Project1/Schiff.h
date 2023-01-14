#ifndef SCHIFF_CLASS_H
#define SCHIFF_CLASS_H

#include "Brett.h"

class Schiff {
public:
	Schiff(int size);
	void checkHit(int zeile, int spalte);
    void setPosition(int startX, int endX, int startY, int endY);
    void setOrientation(char orientation);
    bool contains(int x, int y);
    void hitted();
    bool isVersenkt();
    void replace(Brett* brett);
private:
	int startX;
    int endX;
    int startY;
    int endY;
    int size;
    int hitpoints;
    bool versenkt = false;
    int orientation;
};

#endif