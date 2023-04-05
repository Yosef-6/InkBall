#pragma once
#include "State.h"
#include "Level.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <list>
#include <unordered_set>




class GameState : public State   //directly implement all the logic here  inkball
{
public:
	GameState(StateStack &stack , sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~GameState();
private:
	bool m_set;
	Level m_level;
	unsigned long m_score;
	std::vector<sf::Vector2u> coll;
	sf::Vector2i m_lastMousePos;
	bool m_pressed;
	const sf::Vector2i mOffset{-5, -25};
	std::list<sf::VertexArray> m_vertices;
	std::list<sf::VertexArray>::iterator m_remove;
	std::unordered_set< sf::VertexArray*> m_lineSegments[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE]{};

};


