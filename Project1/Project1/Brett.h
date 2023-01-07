#pragma once
class Brett {
public:
	Brett(int pid);
	void printBrett();
	int field[10][10]; //0: Wasser //1: Schiff //2: Treffer //3: Versenkt //4: daneben
private:
	int playerid;
};
