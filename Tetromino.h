#ifndef TETROMINO_H
#define TETROMINO_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Grid.h"
#include <vector>
#include <SFML/Window/Event.hpp>

class Tetromino {
public:
	enum Direction { None=0, Right=1, Left=-1 };
	enum Rotation { Zero, CW, CCW };
	
	Tetromino() = delete;
	Tetromino(const std::string & shape, sf::Vector2i start_pos, sf::Vector2f blocksize);
	bool IsDone() const;
	void HandleInput();
	void Update(Grid * grid);
	void Render(sf::RenderWindow * window) const;
	void Rotate(Tetromino::Rotation how);
	void Move(Tetromino::Direction where);
	int GetSpeed() { return m_speed; }
	
	inline static sf::Color colors[3] = { sf::Color::Red, sf::Color::Magenta, sf::Color::Blue };
private:
	void setPosition(Grid *grid);
	void setupBlock(int which, const sf::Color & c, const sf::Vector2f & s);
	
	bool m_done;
	int m_speed;
	Tetromino::Direction m_dir;
	Tetromino::Rotation m_rot;
	std::vector<sf::Vector2i> m_gridpos;
	std::vector<sf::RectangleShape> m_model;
	float m_thickness;
};

#endif
