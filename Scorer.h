#ifndef SCORER_H
#define SCORER_H
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Scorer {
public:
	Scorer();
	void Update();
	void Render(sf::RenderWindow *window);
	int GetLevel() const { return m_totalcleared/10 + 1; }
	int ClearedLines;
private:
	sf::RectangleShape m_backdrop;
	int m_totalcleared;
	int m_score;
	std::string m_content;
	sf::Text m_text;
	sf::Font m_font;
};

#endif

