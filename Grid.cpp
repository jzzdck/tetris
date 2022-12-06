#include "Grid.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Grid::Grid() : 
	Grid(sf::Vector2f{5.f,5.f}, sf::Vector2f{350.f,700.f}, sf::Vector2u{10,20})
{
	
}

Grid::Grid (sf::Vector2f start_pos, sf::Vector2f size, sf::Vector2u dim) :
	m_origin(start_pos), m_backdrop(size), m_dims(dim),
	m_blocksize(sf::Vector2f{size.x/dim.x, size.y/dim.y}), m_score(0)
{
	m_backdrop.setPosition(m_origin);
	m_backdrop.setFillColor(sf::Color::Black);
}

void Grid::Update ( ) {
	int lines_cleared = 0;
	for(int i=0;i<m_dims.y;i++) { 
		float y_pos = GetPosition({0,i}).y;
		int cols_occupied = std::count_if(m_occupied.begin(),m_occupied.end(),
										  [&](const sf::RectangleShape &rect) {
											  return rect.getPosition().y == y_pos; 
										  });
		if (cols_occupied == m_dims.x) {
			++lines_cleared;
			this->clearRow(i);
			this->descendBlocks(i-1);
		}
	}
	
	int prev_score = m_score;
	m_score += lines_cleared * lines_cleared * 17 * 13;
	if (m_score != prev_score) std::cout << "SCORE: " << m_score << std::endl;
}

void Grid::Draw (sf::RenderWindow * window) const {
	window->draw(m_backdrop);
	for(size_t i=0;i<m_occupied.size();i++) { 
		window->draw(m_occupied[i]);
	}
}

void Grid::clearRow ( int which ) {
	float row_ypos = m_blocksize.y*which + m_origin.y;
	auto it = std::remove_if(m_occupied.begin(),m_occupied.end(),
							 [&](const sf::RectangleShape &rect) {
								return rect.getPosition().y == row_ypos;
							 });
	
	m_occupied.erase(it, m_occupied.end());
}

void Grid::AddRectangles (const std::vector<sf::RectangleShape> & to_add, const std::vector<sf::Vector2i> & gridpos) {
	for(size_t i=0;i<to_add.size();i++) { 
		m_occupied.push_back(to_add[i]);
	}
}

const sf::Vector2f & Grid::GetBlocksize ( ) {
	return m_blocksize;
}

bool Grid::AssertValidPosition (const sf::Vector2i & gridpos) const {
	auto real_pos = this->GetPosition(gridpos);
	auto is_occup = find_if(m_occupied.begin(),m_occupied.end(),
							[&](const sf::RectangleShape &rect) {
								return rect.getPosition() == real_pos;
							});
	
	bool not_occupied = ( is_occup == m_occupied.end() );
	return (not_occupied and real_pos.y < GetPosition(m_dims).y);
}

sf::Vector2f Grid::GetPosition (const sf::Vector2i & gridpos) const {
	return { gridpos.x * m_blocksize.x + m_origin.x, gridpos.y * m_blocksize.y + m_origin.y};
}

bool Grid::AssertValidCol (const sf::Vector2i & gridpos) const {
	auto real_pos = GetPosition(gridpos);
	bool in_range = ( real_pos.x >= GetPosition({0,0}).x && real_pos.x < GetPosition(m_dims).x );
	return this->AssertValidPosition(gridpos) && in_range;
}

void Grid::descendBlocks (int from) {
	for( auto rect=m_occupied.begin(); rect!=m_occupied.end(); ++rect ) {
		auto pos = rect->getPosition();
		if (pos.y > from * m_blocksize.y + m_origin.y) continue;
		rect->setPosition(sf::Vector2f(pos.x, pos.y + m_blocksize.y));
	}
}

