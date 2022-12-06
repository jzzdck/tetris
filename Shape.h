#ifndef SHAPE_H
#define SHAPE_H

class Shape {
public:
	virtual void Render(Rotation r) const = 0;
private:
	sf::RectangleShape m_model[4];
};

#endif

