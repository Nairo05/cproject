#include <iostream>
#include <vector>
#include <time.h>
#include "Brett.h"
#include "Schiff.h"

std::vector<Schiff *> spielerSchiffe;
std::vector<Schiff *> kiSchiffe;

/**/
int schiffeSetzen();
int kiSchiffeSetzen(int size);
int generateRandom(int max);
int registerToField(int sX, int eX, int sY, int eY, int o);
/**/

Brett *spielerBrett;
Brett *kiBrett;

int main() {
	spielerBrett = new Brett{1}; //Mensch
	spielerBrett -> printBrett();

	kiBrett = new Brett{2}; //KI
	kiBrett -> printBrett();
	
	schiffeSetzen();

	kiBrett -> printBrett();

	return 0;
}

int schiffeSetzen() { //1x2er 3x3er 1x4er 1x5er 
	//Automatisches Setzen der Schiffe für KI
	kiSchiffeSetzen(5);
	kiSchiffeSetzen(4);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(2);

	//Setzen der Schiffe für den Spieler
	//spielerSchiffeSetzen();
	
	return 0;
}

int kiSchiffeSetzen(int shipSize) {
	int startX;
	int startY;
	int endX;
	int endY;
	
	bool ueberschneidung = false;
	do {
		int x = generateRandom(10);
		int y = generateRandom(10);
		int orientation = generateRandom(2);

		if(orientation == 0) {
			if ((x + shipSize) > 9) {
				endX = x;
				startX = x-shipSize;
			} else {
				endX = x+shipSize;
				startX = x;
			}
			startY = y;
			endY = y;
		} else if (orientation == 1) {
			if ((y + shipSize) > 9) {
				endY = y;
				startY = y-shipSize;
			} else {
				endY = y+shipSize;
				startY = y;
			}
			startX = x;
			endX = x; 
		}
		
		//Prüfen auf Überschneidung:
		ueberschneidung = false;
		if (orientation == 0) {
			for (int i = startX; i < endX; i++) {
				if(kiBrett -> field[i][startY] > 0) {
					ueberschneidung = true;
				}
			}
		} else if (orientation == 1) {
			for (int i = startY; i < endY; i++) {
				if(kiBrett -> field[startX][i] > 0) {
					ueberschneidung = true;
				}
			}
		}

		if(!ueberschneidung) {
			registerToField(startX, endX, startY, endY, orientation);
		}
	} while (ueberschneidung);
	 
	Schiff *schiff = new Schiff(shipSize);
	schiff -> setPosition(startX, endX, startY, endY);
	kiSchiffe.push_back(schiff);

	return 0;
}

int generateRandom(int max) {
	srand((int) time(0));
	return rand() % max;
}

int registerToField(int startX, int endX, int startY, int endY, int orientation) {
	if (orientation == 0) {
		for (int i = startX; i < endX; i++) {
			kiBrett -> field[i][startY] = 1; 
		}
	} else if (orientation == 1) {
		for (int i = startY; i < endY; i++) {
			kiBrett -> field[startX][i] = 1; 
		}
	}
	return 0;
}