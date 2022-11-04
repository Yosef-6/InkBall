#pragma once
#include "State.h"
#include "Level.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <list>
#include <unordered_set>

struct keyVal {
	//std::list<sf::VertexArray>::const_iterator it;
	size_t id;
};

namespace std {

	template<>
	struct hash<keyVal> {
		size_t  operator()(const keyVal & k) {

			return hash<size_t>()(k.id);
		}
	};
}


class GameState : public State   //directly implement all the logic here  inkball
{
public:
	GameState(StateStack &stack , sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~GameState();
private:
	Level m_level;
	unsigned long m_score;
	std::vector<sf::Vector2u> coll;
	sf::Vector2i m_lastMousePos;
	bool m_pressed;
	const sf::Vector2i mOffset;
	std::list<sf::VertexArray> m_vertices;
	std::unordered_set<keyVal> m_lineSegments[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE];
};

