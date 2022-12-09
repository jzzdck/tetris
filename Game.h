#ifndef GAME_H
#define GAME_H
#include "Tetromino.h"
#include "Grid.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

class Game {
public:
	Game();
	~Game();
	void Run();
private:
	void makeNewTetromino();
	void update();
	void handleInput();
	void render();
	void clear();
	void loadInfo();
	
	Tetromino *m_tetromino;
	Grid m_grid;
	sf::RenderWindow m_window;
	
	sf::Clock m_clock;
	sf::Time m_elapsed = m_clock.restart();
	float m_frametime;
	
	const std::vector<std::vector<unsigned int>> palletes{ 
		{ 0xFFE829ff, 0xFF0F0Cff, 0xFF7800ff, 0x2C6FD2ff, 0x6AFF3Cff, 0x63145bff, 0x569fb1ff} 
	};
	
	const std::vector<std::string> shapes = { 
		"RUL", "URU", "RUU", "LUU", "ULU", "ELU", "UUU"
	} ;
};

#endif

