#include "Tetromino.h"
#include <stdexcept>
#include <algorithm>

Tetromino::Tetromino (bool disable_rot, const std::string & shape,const sf::Color &c, sf::Vector2i start_pos, sf::Vector2f blocksize) : 
	m_speed(1), m_thickness(4.f), m_hardrop(false), m_waitime(5),
	m_rot(disable_rot ? Rotation::Disabled : Rotation::Zero), 
	m_dir(Direction::None), m_tickcount(0), m_ticklimit(5)
{
	const size_t SHAPE_SIZE = shape.size() + 1;
	m_gridpos.resize(SHAPE_SIZE);
	m_model.resize(SHAPE_SIZE);
	
	const sf::Color & color = c;
	this->setupBlock(0, color, blocksize);
	m_gridpos[0] = start_pos;
	
	for(size_t i=1;i<SHAPE_SIZE;i++) { 
		this->setupBlock(i, color, blocksize);
		m_gridpos[i] = m_gridpos[i-1];
		
		switch(shape[i-1]) {
		case 'U': --m_gridpos[i].y; break;
		case 'R': ++m_gridpos[i].x; break;
		case 'L': --m_gridpos[i].x; break;
		case 'E': ++m_gridpos[i].x, --m_gridpos[i].y; break;
		}
	}
}

void Tetromino::Update (Grid * grid) {
	m_tickcount++;
	
	Shape original = m_gridpos;
	m_gridpos = grid->GetBoundedShape(this->transformShape());
	Shape ghost = grid->GetGhostShape(m_gridpos);
	
	if (m_hardrop or this->lowerThan(ghost)) {
		m_gridpos = grid->assertValidShape(ghost) ? ghost : original;
		m_waitime = m_hardrop ? 0 : (--m_waitime);
	} else { m_waitime = 5; }
	
	m_dir = Tetromino::Direction::None;
	m_rot = (m_rot == Rotation::Disabled ? Rotation::Disabled : Rotation::Zero);
	this->setPosition(grid);
	
	if (!m_waitime) { grid->AddRectangles(m_model); }
}

void Tetromino::HandleInput ( ) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and m_speed < 2) {
		m_speed = 2;
	} else {
		m_speed = 1;
	}
}

void Tetromino::setPosition (Grid * grid) {
	for(size_t i=0;i<m_model.size();i++) { 
		m_model[i].setPosition(grid->GetPosition(m_gridpos[i]));
	}
}

void Tetromino::Rotate ( Tetromino::Rotation how ) {
	m_rot = (m_rot == Rotation::Disabled ? Rotation::Disabled : how);
}

void Tetromino::Render (sf::RenderWindow * window) const {
	for(size_t i=0;i<m_model.size();i++) {
		window->draw(m_model[i]);
	}
}

void Tetromino::setupBlock (int which, const sf::Color & c, const sf::Vector2f & s) {
	m_model[which].setSize({s.x-m_thickness,s.y-m_thickness});
	m_model[which].setOutlineThickness(m_thickness);
	m_model[which].setFillColor(c);
	m_model[which].setOutlineColor(sf::Color::White);
}

void Tetromino::Move (Tetromino::Direction where) {
	if (where != Tetromino::Direction::Right and where != Tetromino::Direction::Left) {
		throw std::invalid_argument("invalid tetromino direction");
	} else {
		m_dir = where;
	}
}

bool Tetromino::IsDone ( ) const {
	return !m_waitime;
}

void Tetromino::HardDrop ( ) {
	m_hardrop = true;
}

Shape Tetromino::transformShape ( ) const {
	std::vector<sf::Vector2i> new_pos = m_gridpos;
	auto origin = m_gridpos[2];
	auto speed = (m_tickcount%m_ticklimit==0 ? m_speed : 0);
	
	for(size_t i=0;i<m_gridpos.size();i++) { 
		if (m_rot != Rotation::Zero and m_rot != Rotation::Disabled) {
			new_pos[i] -= origin;
			new_pos[i] = sf::Vector2i{ -new_pos[i].y, new_pos[i].x };
			new_pos[i] *= (int)m_rot;
			new_pos[i] += origin;
		}
		
		new_pos[i].x += m_dir;
		new_pos[i].y += speed;
	}
	
	return new_pos;
}

bool Tetromino::lowerThan (const Shape & another) const {
	for(size_t i=0;i<m_gridpos.size();i++) { 
		if (m_gridpos[i].y >= another[i].y) {
			return true;
		}
	}
	
	return false;
}
