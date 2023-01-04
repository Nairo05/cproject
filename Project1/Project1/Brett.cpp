#include "Brett.h"
#include <iostream>
Brett::Brett(int pid) {
	playerid = pid;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = 0;
		}
	}
}

void Brett::printBrett() {
	std::cout << "Feld von Spieler " << playerid << std::endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::cout << field[i][j];
			std::cout << " ";
		}
		std::cout << std::endl;
	}

}