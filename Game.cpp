#include "Game.h"
#include <SFML/Window/Event.hpp>

Game::Game() : 
	m_window(sf::VideoMode(360.f,710.f), "tetris"), m_frametime(6.f)
{
	m_window.setFramerateLimit(60);
	m_window.setKeyRepeatEnabled(false);
	this->makeNewTetromino();
}

void Game::Run ( ) {
	while(m_window.isOpen()) {
		this->handleInput();
		this->update();
		this->render();
		m_elapsed += m_clock.restart();
	}
}

void Game::render ( ) {
	m_window.clear(sf::Color(255,255,255,255));
	m_grid.Draw(&m_window);
	m_tetromino->Render(&m_window);
	m_window.display();
}

void Game::clear ( ) {
	delete m_tetromino;
}

void Game::handleInput ( ) {
	sf::Event e;
	
	while(m_window.pollEvent(e)) {
		if(e.type == sf::Event::Closed)
			m_window.close();	
		else if (e.type == sf::Event::KeyPressed) {
			if (e.key.code == sf::Keyboard::R) {
				m_tetromino->Rotate(Tetromino::Rotation::CW);
			} else if (e.key.code == sf::Keyboard::T) {
				m_tetromino->Rotate(Tetromino::Rotation::CCW);
			} else if (e.key.code == sf::Keyboard::Left) {
				m_tetromino->Move(Tetromino::Direction::Left);
			} else if (e.key.code == sf::Keyboard::Right) {
				m_tetromino->Move(Tetromino::Direction::Right);
			}
		}
	}
	
	m_tetromino->HandleInput();
}

void Game::update ( ) {
	float update_ps = 1.f / (m_tetromino->GetSpeed() * m_frametime);
	
	if (m_elapsed.asSeconds() >= update_ps) {
		if (m_tetromino->IsDone()) {
			this->makeNewTetromino();
		} 
		
		m_tetromino->Update(&m_grid);
		m_grid.Update();
		m_elapsed -= sf::seconds(update_ps);
	}
}

Game::~Game ( ) {
	this->clear();
}

void Game::makeNewTetromino ( ) {
	m_tetromino = new Tetromino(shapes[rand()%7], {5,0}, m_grid.GetBlocksize());
}

