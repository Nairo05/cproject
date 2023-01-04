#pragma once
class Schiff {
public:
	Schiff(int size);
	void checkHit(int zeile, int spalte);
    void setPosition(int startX, int endX, int startY, int endY);

private:
	int startX;
    int endX;
    int startY;
    int endY;
    int size;
    bool versenkt = false;
};