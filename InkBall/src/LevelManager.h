#pragma once
#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <functional>

class GameState;



class LevelManager : public State
{

public:
	LevelManager(StateStack& stack, sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~LevelManager() {};
	inline  int getLevelPointer() { return m_levelPointer; }
	std::vector<std::string>m_levelFile;
private:
	void relodLevel();
	sf::RenderTexture m_preview;
	sf::Text m_levelName, m_level;
	sf::Vector2f m_pos{ 0,0 };
	sf::Sound m_click,m_hover;
	sf::Sprite m_backg , m_arrowRight,m_arrowLeft ,m_prev;
	bool m_pressed;
	std::function<GameState* ()>m_game;
	size_t m_levelPointer = 0;

};

