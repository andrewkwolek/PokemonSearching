// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "poke.h"


int main(int argc, char* argv[]) {
	poke Pokemon;

	Pokemon.setArguments(argc, argv);

	Pokemon.readInput();

	if (Pokemon.getMode() == 'M') {
		Pokemon.MST();
		return 0;
	}
	else if (Pokemon.getMode() == 'F') {
		Pokemon.FASTTSP();
		return 0;
	}
	else if (Pokemon.getMode() == 'O') {
		Pokemon.OPTTSP();
		return 0;
	}

	return 0;
}