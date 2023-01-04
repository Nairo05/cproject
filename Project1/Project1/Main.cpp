#include <iostream>
#include "Brett.h"

int main() {
	Brett *b1 = new Brett{1};
	b1 -> printBrett();

	Brett *b2 = new Brett{2};
	b2 -> printBrett();

	return 0;
}