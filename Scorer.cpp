#include "Scorer.h"
#include <sstream>

Scorer::Scorer() {
	auto position = sf::Vector2f(380.f,10.f);
	m_totalcleared = 0;
	m_score = 0;
	ClearedLines = 0;
	
	m_font.loadFromFile("DePixelHalbfett.ttf");
	m_text.setFont(m_font);
	m_text.setString("");
	m_text.setCharacterSize(15);
	m_text.setFillColor(sf::Color::White);
	sf::Vector2f offset(10.f,10.f);
	m_text.setPosition(position + offset);
	
	m_backdrop.setSize({200.f, float(3 * (15 + offset.y/2.f))});
	m_backdrop.setFillColor(sf::Color::Black);
	m_backdrop.setPosition(position);
}

void Scorer::Update ( ) {
	int curr_level = GetLevel();
	int prev_score = m_score;
	
	m_totalcleared += ClearedLines;
	m_score += std::max((200*ClearedLines - 100 + ClearedLines/4 * 100) * GetLevel(),0);
	ClearedLines = 0;
}

void Scorer::Render ( sf::RenderWindow *window ) {
	std::stringstream ss;
	ss << "SCORE: " << m_score << std::endl;
	float linesize=ss.str().size();
	ss << "LINES: " << m_totalcleared << std::endl;
	ss << "LV. " << GetLevel() << std::endl;
	
	float charsize = m_backdrop.getSize().x/linesize;
	m_text.setCharacterSize(charsize);
	m_backdrop.setSize({200.f, float(4 * (charsize +10.f/2.f))});
	m_text.setString(ss.str());
	
	window->draw(m_backdrop);
	window->draw(m_text);
}

