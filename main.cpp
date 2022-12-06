#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Game.h"

int main(int argc, char *argv[]){
	std::srand(std::time(nullptr));
	Game g;
	g.Run();
	return 0;
}

