#include "Tetromino.h"
#include <stdexcept>

void Tetromino::Update (Grid * grid) {
	auto origin = m_gridpos[1];
	std::vector<sf::Vector2i> new_pos = m_gridpos;
	
	for(int i=0;i<m_gridpos.size();i++) { 
		if (m_rot != Tetromino::Rotation::Zero) {
			new_pos[i] -= origin;
			new_pos[i] = sf::Vector2i{ -new_pos[i].y, new_pos[i].x };
			if (m_rot == Tetromino::Rotation::CW) new_pos[i] *= -1;
			new_pos[i] += origin;
		}
		
		// si la rotacion me vuelve invalida la posicion, deshacerla
		if (!grid->AssertValidCol(new_pos[i])) {
			new_pos = m_gridpos;
			m_rot = Tetromino::Rotation::Zero;
		}
		
		// si mover el tetromino invalida la posicion, deshacer el movimiento
		new_pos[i].x += m_dir;
		if (!grid->AssertValidCol(new_pos[i])) {
			for(int j=0;j<=i;j++) { 
				new_pos[j].x -= m_dir;
			}
			m_dir = Tetromino::Direction::None;
		}
		
		// si la caida me vuelve invalida la posicion, 
		// es porque choco contra un tetromino o el piso,
		// entonces hay que frenar el tetromino
		new_pos[i].y += 1;
		if (!grid->AssertValidPosition(new_pos[i])) {
			m_done = true;
			break;
		}
	}
	
	m_dir = Tetromino::Direction::None;
	m_rot = Tetromino::Rotation::Zero;
	if (!m_done) {
		m_gridpos = new_pos;
		this->setPosition(grid);
	} else {
		grid->AddRectangles(m_model,m_gridpos);
	}
}

void Tetromino::HandleInput ( ) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		m_speed = 2;
	} else {
		m_speed = 1;
	}
}

Tetromino::Tetromino (const std::string & shape, sf::Vector2i start_pos, sf::Vector2f blocksize) : 
	m_speed(0), m_thickness(4.f), m_done(false),
	m_rot(Tetromino::Rotation::Zero),
	m_dir(Tetromino::Direction::None)
{
	const size_t SHAPE_SIZE = shape.size() + 1;
	m_gridpos.resize(SHAPE_SIZE);
	m_model.resize(SHAPE_SIZE);
	
	const sf::Color & color = colors[rand()%3];
	this->setupBlock(0, color, blocksize);
	m_gridpos[0] = start_pos;
	
	for(size_t i=1;i<SHAPE_SIZE;i++) { 
		this->setupBlock(i, color, blocksize);
		m_gridpos[i] = m_gridpos[i-1];
		
		switch(shape[i-1]) {
		case 'U': --m_gridpos[i].y; break;
		case 'R': ++m_gridpos[i].x; break;
		case 'L': --m_gridpos[i].x; break;
		case 'E': ++m_gridpos[i].x, ++m_gridpos[i].y; break;
		}
	}
}

void Tetromino::setPosition (Grid * grid) {
	for(size_t i=0;i<m_model.size();i++) { 
		m_model[i].setPosition(grid->GetPosition(m_gridpos[i]));
	}
}

void Tetromino::Rotate ( Tetromino::Rotation how ) {
	m_rot = how;
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
	return m_done;
}

