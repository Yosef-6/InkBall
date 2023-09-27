#pragma once
#include "State.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Clock.hpp"

class MessageState :  public State
{
public:
	MessageState(StateStack& stack, sf::RenderWindow* window,Inkball::Mode);
	virtual bool  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~MessageState() {};
private:
	Inkball::Mode m_mode;
	sf::Sprite m_backg;
	sf::Text m_message , m_confirmation;

	sf::Clock clock;
};

