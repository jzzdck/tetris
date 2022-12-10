#ifndef GRID_H
#define GRID_H
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
using Shape = std::vector<sf::Vector2i>;

class Grid {
public:
	Grid();
	Grid(sf::Vector2f start_pos, sf::Vector2f size, sf::Vector2u dim);
	void Update(int &lines_cleared);
	void AddRectangles(const std::vector<sf::RectangleShape> &to_add);
	void Draw(sf::RenderWindow * window) const;
	const sf::Vector2f & GetBlocksize();
	sf::Vector2f GetPosition(const sf::Vector2i & gridpos) const;
	Shape GetGhostShape(const Shape &shape) const;
	Shape GetBoundedShape(const Shape &shape) const;
	bool assertValidShape(const Shape &shape) const;
private:
	bool assertValidCol(const sf::Vector2i & gridpos) const;
	void descendBlocks(int from);
	void clearRow(int which);
	sf::RectangleShape m_backdrop;
	sf::Vector2i m_dims;
	sf::Vector2f m_origin;
	sf::Vector2f m_blocksize;
	std::vector<sf::RectangleShape> m_occupied;
};

#endif

