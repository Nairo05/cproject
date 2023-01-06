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
int spielerSchiffeSetzen(int size);
int generateRandom(int max);
int registerToField(int sX, int eX, int sY, int eY, int o, Brett* brett);
/**/

Brett *spielerBrett;
Brett *kiBrett;

int main() {
	spielerBrett = new Brett{1};
	kiBrett = new Brett{2};
	
	std::cout << std::endl << "[ SCHIFFE VERSENKEN ]" << std::endl;
	std::cout << "Kannst du es schaffen, alle Schiffe der KI zu versenken, bevor sie dich besiegt?" << std::endl;
	std::cout << "Das Spiel folgt den bekannten Regeln und bezieht sich auf folgendes Feld: " << std::endl << std::endl;
	kiBrett -> printBrett();
	std::cout << std::endl << "Viel Erfolg!" << std::endl << std::endl;
	
	schiffeSetzen();

	spielerBrett -> printBrett();
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
	spielerSchiffeSetzen(5);
	spielerSchiffeSetzen(4);
	spielerSchiffeSetzen(3);
	spielerSchiffeSetzen(3);
	spielerSchiffeSetzen(3);
	spielerSchiffeSetzen(2);
	
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
			if ((x + shipSize) > 10) {
				endX = x;
				startX = x-shipSize+1;
			} else {
				endX = x+shipSize-1;
				startX = x;
			}
			startY = y;
			endY = y;
		} else if (orientation == 1) {
			if ((y + shipSize) > 10) {
				endY = y;
				startY = y-shipSize+1;
			} else {
				endY = y+shipSize-1;
				startY = y;
			}
			startX = x;
			endX = x; 
		}
		
		//Prüfen auf Überschneidung:
		ueberschneidung = false;
		if (orientation == 0) {
			for (int i = startX; i <= endX; i++) {
				if(kiBrett -> field[startY][i] > 0) {
					ueberschneidung = true;
				}
			}
		} else if (orientation == 1) {
			for (int i = startY; i <= endY; i++) {
				if(kiBrett -> field[i][startX] > 0) {
					ueberschneidung = true;
				}
			}
		}

		if(!ueberschneidung) {
			registerToField(startX, endX, startY, endY, orientation, kiBrett);
		}
	} while (ueberschneidung);
	 
	Schiff *schiff = new Schiff(shipSize);
	schiff -> setPosition(startX, endX, startY, endY);
	kiSchiffe.push_back(schiff);

	return 0;
}

int spielerSchiffeSetzen(int shipSize) {
	
	char o;
	int zeile;
	int spalte; 
	bool unzulaessig;
	do {
		unzulaessig = false;
		std::cout << std::endl << "---[Setze ein Schiff der Laenge " << shipSize << "]---" << std::endl;
		std::cout << "[ Lege die Orientierung fest: Horizontal (h) oder Vertikal (v) ]" << std::endl;
		std::cin >> o;
		std::cout << "[ Gib nun die Position des Schiffsteils an, der am weitesten links unten sein soll (z.B. B4) ]" << std::endl;
		std::string p;
		std::cin >> p;

		//Extrahieren der Eingabekoordinaten in zwei ints.
		if (p.length() == 2) {
			try {
				zeile = p[0] - 65;
				spalte = p[1] - 48;
			} catch (...) {
				unzulaessig = true;
			}
		} else {
			unzulaessig = true;
		}
		if (!unzulaessig) {
			if (o == 'h') {
				if (zeile > 9 || zeile < 0 || spalte > 9 || spalte < 0 || (spalte+shipSize) > 10) {
					unzulaessig = true;
				} else {

				}
			} else if (o == 'v') {
				if (zeile > 9 || zeile < 0 || spalte > 9 || spalte < 0 || (zeile-shipSize) < -1) {
					unzulaessig = true;
				}
			} else {
				unzulaessig = true; //in dem Fall hat der Spieler was anderes als 'h' oder 'v' angegeben.
			}
		}
		if (!unzulaessig) {
			//Prüfen, ob das Schiff mit anderen kollidiert
			if (o == 'h') {
			for (int i = spalte; i < spalte+shipSize; i++) {
				if(spielerBrett -> field[zeile][i] > 0) {
					unzulaessig = true;
				}
			}
		} else if (o == 'v') {
			for (int i = zeile; i > zeile-shipSize; i--) {
				if(spielerBrett -> field[i][spalte] > 0) {
					unzulaessig = true;
				}
			}
		}
		}

		if (unzulaessig) {
			std::cout << std::endl;
			std::cout << "[ Diese Position ist nicht zulaessig. Beachte bitte folgende Kriterien: ]" << std::endl;
			std::cout << "- Das Schiff darf nicht ueber das Feld hinausragen" << std::endl;
			std::cout << "- Das Schiff darf sich nicht mit anderen Schiffen kreuzen" << std::endl;
			std::cout << "- Beachte das Eingabeformat [Zeile][Spalte], z.B. B4" << std::endl << std::endl;
		}

	} while (unzulaessig);
	
	Schiff *schiff = new Schiff(shipSize);
	int orientation;
	if (o == 'h') {
		orientation = 0;
		registerToField(spalte, spalte+shipSize-1, zeile, zeile, orientation, spielerBrett);
		schiff -> setPosition(spalte, spalte+shipSize-1, zeile, zeile);
	} else {
		orientation = 1;
		registerToField(spalte, spalte, zeile-shipSize+1, zeile, orientation, spielerBrett);
		schiff -> setPosition(spalte, spalte, zeile-shipSize+1, zeile);
	}
	spielerSchiffe.push_back(schiff);
	
	return 0;
}

int generateRandom(int max) {
	srand((int) time(0));
	return rand() % max;
}

int registerToField(int startX, int endX, int startY, int endY, int orientation, Brett* brett) {
	if (orientation == 0) {
		for (int i = startX; i <= endX; i++) {
			brett -> field[startY][i] = 1; 
		}
	} else if (orientation == 1) {
		for (int i = startY; i <= endY; i++) {
			brett -> field[i][startX] = 1; 
		}
	}
	return 0;
}