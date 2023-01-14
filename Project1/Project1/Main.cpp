#include <iostream>
#include <vector>
#include <unistd.h>
#include <random>
#include <time.h>
#include "Brett.h"
#include "Schiff.h"

/**/
int schiffeSetzen();
int kiSchiffeSetzen(int size);
int spielerSchiffeSetzen(int size);
int generateRandom(int max);
int registerToField(int sX, int eX, int sY, int eY, int o, Brett* brett);
int spielStarten();
int spielZug(int naechster);
/**/


enum kiStrategien {zufall, nord, ost, sued, west}; 
kiStrategien strategie = zufall;
int kiTarget_x;
int kiTarget_y;
int cyclicCounter = 0;

std::vector<Schiff *> spielerSchiffe;
std::vector<Schiff *> kiSchiffe;

Brett *spielerBrett;
Brett *kiBrett;

int main() {

	//Wir mussten die pid hinzunehmen, da sonst immer Koordinaten der Form 11, 22, 33, ... rauskamen
	srand(time(NULL)+getpid());

	spielerBrett = new Brett{1};
	kiBrett = new Brett{2};
	
	std::cout << std::endl << "[ SCHIFFE VERSENKEN ]" << std::endl;
	std::cout << "Kannst du es schaffen, alle Schiffe der KI zu versenken, bevor sie dich besiegt?" << std::endl;
	std::cout << "Das Spiel folgt den bekannten Regeln und bezieht sich auf folgendes Feld: " << std::endl << std::endl;
	kiBrett -> printBrett();
	std::cout << std::endl << "Viel Erfolg!" << std::endl << std::endl;
	std::cout << "Das Spielbrett wird nun vorbereitet. Bitte warten..." << std::endl << std::endl;
	
	schiffeSetzen();

	spielerBrett -> printBrett();
	kiBrett -> printBrett();

	int gewinner = spielStarten();

	std::cout << "---[ SPIEL BEENDET ]---" << std::endl;
	if (gewinner == 1) {
		std::cout << "Der Gewinner ist: Spieler " << gewinner << std::endl;
	} else {
		std::cout << "Der Gewinner ist: KI " << std::endl;
	}

	for (int i = 0; i < kiSchiffe.size(); i++) {
		delete kiSchiffe.at(i);
	}
	for (int i = 0; i < spielerSchiffe.size(); i++) {
		delete spielerSchiffe.at(i);
	}
	
	delete spielerBrett;
	delete kiBrett;
	return 0;
}

void clear() {
	system("clear");
	system("cls");
}

int schiffeSetzen() { //1x2er 3x3er 1x4er 1x5er 
	//Automatisches Setzen der Schiffe für KI
	kiSchiffeSetzen(5);
	kiSchiffeSetzen(4);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(3);
	kiSchiffeSetzen(2);

	clear();

	kiBrett -> printBrett();

	//Setzen der Schiffe für den Spieler
	spielerSchiffeSetzen(5);
	clear();
	spielerBrett -> printBrett();

	spielerSchiffeSetzen(4);
	clear();
	spielerBrett -> printBrett();

	spielerSchiffeSetzen(3);
	clear();
	spielerBrett -> printBrett();

	spielerSchiffeSetzen(3);
	clear();
	spielerBrett -> printBrett();

	spielerSchiffeSetzen(3);
	clear();
	spielerBrett -> printBrett();

	spielerSchiffeSetzen(2);

	return 0;
}

int kiSchiffeSetzen(int shipSize) {
	int startX;
	int startY;
	int endX;
	int endY;
	int orientation;
	
	bool ueberschneidung = false;
	do {
		int x = generateRandom(9);
		int y = generateRandom(9);
		orientation = generateRandom(1);

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
		
		//Prüfen auf Überschneidung bzw. Kollission:
		ueberschneidung = false;
		if (orientation == 0) {
			for (int i = startX; i <= endX; i++) {
				if(kiBrett -> field[startY][i] > 0) {
					ueberschneidung = true;
				}
				if (startY != 0) {
					if(kiBrett -> field[startY-1][i] == 1) {
						ueberschneidung = true;
					}
				}
				if (startY != 9) {
					if(kiBrett -> field[startY+1][i] == 1) {
						ueberschneidung = true;
					}
				}
			}
			if (startX != 0) {
				if (kiBrett -> field[startY][startX-1] == 1) {
					ueberschneidung = true;
				}
			}
			if (endX != 9) {
				if (kiBrett -> field[startY][endX+1] == 1) {
					ueberschneidung = true;
				}
			}
		} else if (orientation == 1) {
			for (int i = startY; i <= endY; i++) {
				if(kiBrett -> field[i][startX] > 0) {
					ueberschneidung = true;
				}
				if (startX != 9) {
					if(kiBrett -> field[i][startX+1] == 1) {
						ueberschneidung = true;
					}
				}
				if (startX != 0) {
					if(kiBrett -> field[i][startX-1] == 1) {
						ueberschneidung = true;
					}
				}	
			}
			if (startY != 0) {
				if (kiBrett -> field[startY-1][startX] ==  1) {
					ueberschneidung = true;
				}
			}
			if (endY != 9) {
				if (kiBrett -> field[endY+1][startX] == 1) {
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
	schiff -> setOrientation(orientation);
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
				
				if (zeile !=9) {
					if(spielerBrett -> field[zeile+1][i] == 1) {
						unzulaessig = true;
					}
				}
				
				if (zeile != 0) {
					if(spielerBrett -> field[zeile-1][i] == 1) {
						unzulaessig = true;
					}
				}
				
			}

			if (spalte !=0) {
				if (spielerBrett -> field[zeile][spalte-1] == 1) {
					unzulaessig = true;
				}
			}
			if ((spalte+shipSize-1) != 9) {
				if (spielerBrett -> field[zeile][spalte+shipSize] == 1) {
					unzulaessig = true;
				}
			}
		
			} else if (o == 'v') {
				for (int i = zeile; i > zeile-shipSize; i--) {
					if(spielerBrett -> field[i][spalte] > 0) {
						unzulaessig = true;
					}
					if (spalte != 9) {
						if(spielerBrett -> field[i][spalte+1] == 1) {
							unzulaessig = true;
						}
					}
				
					if (spalte != 0) {
						if(spielerBrett -> field[i][spalte-1] == 1) {
						unzulaessig = true;
					}
					}
				
				}

				if (zeile != 9) {
					if (spielerBrett -> field[zeile+1][spalte] == 1) {
						unzulaessig = true;
					}
				}
				if (zeile != 0) {
					if (spielerBrett -> field[zeile-shipSize][spalte] == 1) {
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
	int min = 0;
    return (rand() % (max - min +1) + min);
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
		//Prüfen auf Spielende, und zwar so: Wenn das Feld des Spielers der jetzt als nächstes dran ist keine 1 mehr enthält, hat der andere Spieler gewonnen!
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
	bool unzulaessig = false;

	if (n == 1) { //Spieler

		int zeile;
		int spalte;
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
			naechster = 1;
		} else {
			//Prüfen, ob ein Schiff auf dem kiBrett getroffen bzw. versenkt wurde:
			switch (kiBrett -> field[zeile][spalte]) {
				case 0:
					std::cout << "Daneben!" << std::endl;
					kiBrett -> field[zeile][spalte] = 4;
					naechster = 2;
					break;
				case 1:
					std::cout << "Treffer!" << std::endl;
					kiBrett -> field[zeile][spalte] = 2;
					naechster = 1; //Spieler ist nochmal dran
					for (int i = 0; i < kiSchiffe.size(); i++) {
						std::cout << "Vektor: " << i << std::endl;
						if (kiSchiffe.at(i) -> contains(spalte, zeile)) {
							std::cout << "enthalten: " << i << std::endl;
							kiSchiffe.at(i) -> hitted();
							if (kiSchiffe.at(i) -> isVersenkt()) {
								kiSchiffe.at(i) -> replace(kiBrett);

								delete kiSchiffe.at(i);

								kiSchiffe.erase(kiSchiffe.begin() + i);
								std::cout << "Versenkt!" << std::endl;
							}
						}
					}
					break;
				default:
					std::cout << "Du hast bereits auf dieses Feld geschossen!" << std::endl;
					naechster = 1;
					break;
			} 
		}
	} else if (n == 2) { //KI
		int zeile;
		int spalte;
		switch (strategie) {
		case nord: 
			if (kiTarget_y != 0) {
				zeile = kiTarget_y - 1;
			} else {
				zeile = kiTarget_y;
				strategie = sued;
			}
			spalte = kiTarget_x;
			break;
		case ost:
			zeile = kiTarget_y;
			if (kiTarget_x != 9) {
				spalte = kiTarget_x + 1;
			} else {
				spalte = kiTarget_x;
				strategie = west;
			}
			break;
		case sued:
			if (kiTarget_y != 9) {
				zeile = kiTarget_y + 1;
			} else {
				zeile = kiTarget_y;
				strategie = nord;
			}
			spalte = kiTarget_x;
			break;
		case west:
			zeile = kiTarget_y;
			if (kiTarget_x != 0) {
				spalte = kiTarget_x - 1;
			} else {
				spalte = kiTarget_x;
				strategie = ost;
			}
			break;
		case zufall:
		default:
			zeile = generateRandom(9);
			spalte = generateRandom(9);
			break;
		}

		if(zeile < 0 || zeile > 9 || spalte < 0 || spalte > 9) {
			strategie = zufall;
			return 2;
		} //Sollte es bei der Koordinatenwahl zu einem Fehler kommen, wird Strategie auf Zufall zurückgesetzt und KI ist nochmal dran.

		switch (spielerBrett -> field[zeile][spalte]) {
			case 0:
				std::cout << "---[ Die KI ist dran - Warum zitterst du so? ]---" << std::endl;
				std::cout << (char)(zeile+65) << spalte << std::endl;
				std::cout << "KI hat daneben geschossen!" << std::endl;
				cyclicCounter = 0;
				spielerBrett -> field[zeile][spalte] = 4;
				if (strategie == west) {
					strategie = ost;
				} else if (strategie == ost) {
					strategie = nord;
				} else if (strategie == nord) {
					strategie = sued;
				} else if (strategie == sued) {
					strategie = west;
				} else {
					strategie = zufall;
				}
				naechster = 1;
				break;
			case 4:
				cyclicCounter++;
				if (cyclicCounter >= 5) {
					cyclicCounter = 0;
					strategie = zufall;
					return 1;
				}
				if (strategie == west) {
					strategie = ost;
				} else if (strategie == ost) {
					strategie = nord;
				} else if (strategie == nord) {
					strategie = sued;
				} else if (strategie == sued) {
					strategie = west;
				} else {
					strategie = zufall;
				}
				naechster = 2;
				break;
			case 1:
				cyclicCounter = 0;
				std::cout << "---[ Die KI ist dran - Warum zitterst du so? ]---" << std::endl;
				std::cout << (char)(zeile+65) << spalte << std::endl;
				std::cout << "KI hat einen Treffer erzielt!" << std::endl;
				kiTarget_x = spalte;
				kiTarget_y = zeile;
				spielerBrett -> field[zeile][spalte] = 2;
				if (strategie == zufall) {
					strategie = west;
				}
				for (int i = 0; i < spielerSchiffe.size(); i++){
					if(spielerSchiffe.at(i) -> contains(kiTarget_x, kiTarget_y)) {
						spielerSchiffe.at(i) -> hitted();
						if (spielerSchiffe.at(i) -> isVersenkt()) {
							std::cout << "Dein Schiff wurde versenkt!" << std::endl;

							spielerSchiffe.at(i) -> replace(spielerBrett);

							delete spielerSchiffe.at(i);

							spielerSchiffe.erase(spielerSchiffe.begin() + i);

							kiTarget_x = 5; //Target vom Rand wegsetzen
							kiTarget_y = 5; //Target vom Rand wegsetzen
							strategie = zufall;
			
						}
					}
				}
				naechster = 2;
				break;
			case 2:
				kiTarget_x = spalte;
				kiTarget_y = zeile;
				naechster = 2;
				break;
			default:
				strategie = zufall;
				naechster = 1;
		}
		
	}

	
	if (!unzulaessig) {
		spielerBrett -> printBrett();
		kiBrett -> printBrett();
	} 
	
	return naechster;
}