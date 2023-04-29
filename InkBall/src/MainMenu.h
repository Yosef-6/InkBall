#pragma once
#include "State.h"
#include <SFML/Audio/Sound.hpp>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>

class GameState;

class MainMenu :  public State
{
public:
	MainMenu(StateStack& stack, sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~MainMenu() {};
private:
	std::vector<sf::Text>m_items;
	sf::Text m_menu;
	unsigned char m_currentItemPointer = 0;
	sf::Vector2f m_pos{0,0};
	sf::Sound m_hover, m_click;
	sf::Sprite m_backg;
	bool m_pressed;
};

