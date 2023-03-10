#include "Brett.h"
#include <iostream>
#include "Schiff.h"

Brett::Brett(int pid) {
	playerid = pid;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = 0;
		}
	}
}


void Brett::printBrett() {
	if (playerid == 1) {
		std::cout << std::endl << "Feld von Spieler " << playerid << ":" << std::endl;
	} else {
		std::cout << std::endl << "Feld von der KI:" << std::endl;
	}
	std::cout << " ";	
	
	for (int i = 0; i < 10; i++) {
		std::cout << " " << i;
	}
	std::cout << std::endl;
	char zeilenBez = 'A';
	for (int i = 0; i < 10; i++) {
		std::cout << (char)(zeilenBez + i) << " ";
		for (int j = 0; j < 10; j++) {
			if((playerid == 2) && (field[i][j] == 1)) {
				std::cout << "~";
			} else if (field[i][j] == 0) {
				std::cout << "~";
			} else if (field[i][j] == 2) {
				std::cout << "X";
			} else if (field[i][j] == 3) {
				std::cout << "#";
			} else if (field[i][j] == 4) {
				std::cout << "%";
			} else {
				std::cout << field[i][j];
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}