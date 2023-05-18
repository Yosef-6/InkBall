#pragma once
#include "State.h"
#include "Level.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <list>
#include <unordered_set>




class GameState : public State   //directly implement all the logic here  inkball
{
public:
	using vertex = std::tuple<sf::VertexArray, bool, bool>;
	GameState(StateStack& stack, sf::RenderWindow* window,std::vector<std::string>&&,size_t );
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~GameState();
private:
	bool m_set;
	Level m_level;
	unsigned long m_score;
	std::vector<sf::Vector2u> coll;
	sf::Vector2f m_lastMousePos;
	bool m_pressed;
	const sf::Vector2i mOffset{-5, -25};
	const float mSeparationCoff = 16.0f;
	std::size_t m_levelPointer;
	std::vector<std::string>m_levelInfo;
	std::list< vertex > m_vertices;
	std::list< vertex >::iterator m_remove;
	std::unordered_set< vertex* > m_lineSegments[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE]{};

};


