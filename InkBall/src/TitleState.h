#pragma once
#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
class TitleState :  public State
{
public:
	TitleState(StateStack& stack, sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~TitleState();
private:
	sf::Sprite m_logo;
	sf::Sprite m_backg;
	sf::Text m_Inkball;
	sf::Text m_text;
	sf::Text m_message;
	bool m_showText;
	sf::Time m_textBlinkTime;
	
};

