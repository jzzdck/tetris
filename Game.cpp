#include "Game.h"
#include <SFML/Window/Event.hpp>
#include <algorithm>

Game::Game() : 
	m_window(sf::VideoMode(600.f,710.f), "tetris"), m_frametime(10.f),
	m_previewgrid(sf::Vector2f{430.f, 200.f},sf::Vector2f{100.f,100.f}, sf::Vector2u{6,6})
{
	m_window.setFramerateLimit(60);
	m_window.setKeyRepeatEnabled(false);
	m_next = rand()%7;
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
	m_window.clear(sf::Color(0x797C80ff));
	m_grid.Draw(&m_window);
	m_previewgrid.Draw(&m_window);
	m_tetromino->Render(&m_window);
	m_preview->Render(&m_window);
	m_scorer.Render(&m_window);
	m_window.display();
}

void Game::clear ( ) {
	delete m_tetromino;
	delete m_preview;
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
			} else if (e.key.code == sf::Keyboard::LControl) {
				m_tetromino->HardDrop();
			}
		}
	}
	
	m_tetromino->HandleInput();
}

void Game::update ( ) {
	float update_ps = 1.f / (m_scorer.GetLevel() * 0.5f * m_tetromino->GetSpeed() * m_frametime);
	
	if (m_elapsed.asSeconds() >= update_ps) {
		if (m_tetromino->IsDone()) {
			this->makeNewTetromino();
		} 
		
		m_tetromino->Update(&m_grid);
		m_grid.Update(m_scorer.ClearedLines);
		m_scorer.Update();
		m_elapsed -= sf::seconds(update_ps);
	}
}

Game::~Game ( ) {
	this->clear();
}

void Game::makeNewTetromino ( ) {
	m_tetromino = new Tetromino(shapes[m_next], sf::Color(palletes[0][m_next]), {5,0}, m_grid.GetBlocksize());
	if (!m_next) m_tetromino->Rotate(Tetromino::Rotation::Disabled);
	m_next = rand()%7;
	
	int yoff = 0, xoff = 0;
	for(size_t i=0;i<shapes[m_next].size();i++) { 
		if (shapes[m_next][i] == 'U') {
			yoff++;
		} 
		
		if (shapes[m_next][i] == 'E') {
			yoff--;
		}
		
		if (shapes[m_next][i] == 'R' or shapes[m_next][i] == 'E') {
			xoff--;
		}
		
		if (shapes[m_next][i] == 'L') {
			xoff++;
		}
	}
	
	m_preview = new Tetromino(shapes[m_next], sf::Color(palletes[0][m_next]), {2+xoff/2,3+yoff/2}, m_previewgrid.GetBlocksize());
	m_preview->Update(&m_previewgrid);
	if (m_next) {
		sf::Vector2f ofs(m_previewgrid.GetBlocksize().x/2.f, 0.f);
		m_preview->Offset(ofs);
	}
}

