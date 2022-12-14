#include "Grid.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>

Grid::Grid() : 
	Grid(sf::Vector2f{15.f,5.f}, sf::Vector2f{350.f,700.f}, sf::Vector2u{10,20})
{  

}

Grid::Grid (sf::Vector2f start_pos, sf::Vector2f size, sf::Vector2u dim) :
	m_origin(start_pos), m_backdrop(size), m_dims(dim),
	m_blocksize(sf::Vector2f{size.x/dim.x, size.y/dim.y})
{
	m_backdrop.setPosition(sf::Vector2f{m_origin.x - 2.f, m_origin.y});
	m_backdrop.setFillColor(sf::Color(0x000000ff));
}

void Grid::Update ( int &lines_cleared ) {
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

void Grid::AddRectangles (const std::vector<sf::RectangleShape> & to_add) {
	for(size_t i=0;i<to_add.size();i++) { 
		m_occupied.push_back(to_add[i]);
	}
}

const sf::Vector2f & Grid::GetBlocksize ( ) {
	return m_blocksize;
}

sf::Vector2f Grid::GetPosition (const sf::Vector2i & gridpos) const {
	return { gridpos.x * m_blocksize.x + m_origin.x, gridpos.y * m_blocksize.y + m_origin.y};
}

bool Grid::assertValidCol (const sf::Vector2i & gridpos) const {
	auto real_pos = this->GetPosition(gridpos);
	auto is_occup = find_if(m_occupied.begin(),m_occupied.end(),
							[&](const sf::RectangleShape &rect) {
								return rect.getPosition() == real_pos;
							});
	
	bool not_occupied = ( is_occup == m_occupied.end() );
	bool in_range = ( real_pos.x >= GetPosition({0,0}).x  and real_pos.y < GetPosition(m_dims).y );
	return (not_occupied and in_range);
}

void Grid::descendBlocks (int from) {
	for( auto rect=m_occupied.begin(); rect!=m_occupied.end(); ++rect ) {
		auto pos = rect->getPosition();
		if (pos.y > from * m_blocksize.y + m_origin.y) continue;
		rect->setPosition(sf::Vector2f(pos.x, pos.y + m_blocksize.y));
	}
}

bool Grid::assertValidShape (const Shape & shape) const {
	for(size_t i=0;i<shape.size();i++) { 
		if (!this->assertValidCol(shape[i])) {
			return false;
		}
	}
	
	return true;
}

Shape Grid::GetGhostShape (const Shape & shape) const {
	Shape ghost_shape = shape;
	
	while (assertValidShape(ghost_shape)) {
		for(size_t i=0;i<ghost_shape.size();i++) { 
			++ghost_shape[i].y;
		}
	}
	
	for(size_t i=0;i<ghost_shape.size();i++) { 
		--ghost_shape[i].y;
	}
	
	return ghost_shape;
}

Shape Grid::GetBoundedShape (const Shape & shape) const {
	Shape ghost_shape = shape;
	int x_dif = 0;
	for(size_t i=0;i<ghost_shape.size();i++) { 
		int adif = m_dims.x - ghost_shape[i].x;
		if (adif <= 0) {
			x_dif = std::min(adif-1,x_dif);
		} else if (adif > m_dims.x) {
			x_dif = std::max(adif - m_dims.x,x_dif);
		}
	}
	
	if (x_dif != 0) {
		for(size_t i=0;i<ghost_shape.size();i++) { 
			ghost_shape[i].x += x_dif;
		}
	}
	
	return ghost_shape;
}

