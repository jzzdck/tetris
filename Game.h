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
	
	inline static std::vector<std::string> shapes = { "RUL", "URU", "RUU", "LUU", "ULU", "UUE", "UUU" } ;
};

#endif

