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
int spielStarten();
int spielZug(int naechster);
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

	int gewinner = spielStarten();

	std::cout << "---[ SPIEL BEENDET ]---" << std::endl;
	std::cout << "Der Gewinner ist: Spieler " << gewinner << std::endl;
	
	delete spielerBrett;
	delete kiBrett;
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
	 
	Schiff *schiff = new Schiff(shipSize); //TODO: delete
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
	
	Schiff *schiff = new Schiff(shipSize); //TODO: delete
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

int spielStarten() {
	int spielzugCounter = 0;
	bool spielEnde = false;
	int vorheriger;
	int naechster = spielZug(1); //Spieler darf zuerst ziehen.
	while (!spielEnde) {
		vorheriger = naechster;
		std::cout << "Vorheriger: "<< vorheriger << std::endl;
		naechster = spielZug(naechster);
		spielzugCounter++;
		//TODO: Prüfen auf Spielende, und zwar so: Wenn das Feld des Spielers der jetzt als nächstes dran ist keine 1 mehr enthält, hat der andere Spieler gewonnen!
		spielEnde = true;
		if (vorheriger == 1) {
			for(int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (kiBrett -> field[i][j] == 1) {
						spielEnde = false;
						break;
					}
				}
			}
		} else {
			for(int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (spielerBrett -> field[i][j] == 1) {
						spielEnde = false;
						break;
					}
				}
			}
		}
	}
	
	return vorheriger;
}

int spielZug(int n) {
	int naechster;
	int zeile;
	int spalte;
	bool unzulaessig = false;

	if (n == 1) { //Spieler
		std::cout << "---[ Du bist an der Reihe - Nenne eine Koordinate zum Beschuss ]---" << std::endl;
		
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
			if (zeile > 9 || zeile < 0 || spalte > 9 || spalte < 0) {
					unzulaessig = true;
			} 
		}
		if (unzulaessig) {
			std::cout << "---[ Eingabe falsch, versuche es nochmal ]---" << std::endl;
			naechster = 1; //Spieler ist nochmal dran.
		} else {
			//Prüfen, ob ein Schiff auf dem kiBrett getroffen bzw. versenkt wurde:
			switch (kiBrett -> field[zeile][spalte]) {
				case 0:
					std::cout << "Daneben!" << std::endl;
					kiBrett -> field[zeile][spalte] = 4;
					naechster = 2; //KI ist dran
					break;
				case 1:
					std::cout << "Treffer!" << std::endl;
					kiBrett -> field[zeile][spalte] = 2;
					naechster = 1; //Spieler ist nochmal dran
					//TODO: Prüfen auf "Versenkt"
					break;
				default:
					std::cout << "Du hast bereits auf dieses Feld geschossen!" << std::endl;
					naechster = 1; //Spieler ist nochmal dran weil es gemein ist, Dummheit zu bestrafen :/
					break;
			} 
		}
	} else if (n == 2) { //KI
		std::cout << "---[ Die KI ist dran - Warum zitterst du so? ]---" << std::endl;
		
		std::cout << "A1 wird beschossen." << std::endl;
		std::cout << "Daneben!" << std::endl;
		naechster = 1; 
	}

	spielerBrett -> printBrett();
	kiBrett -> printBrett();
	return naechster;
}