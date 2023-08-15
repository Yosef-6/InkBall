#pragma once
#include "Entity.h"
#include "Identifiers.h"
#include <SFML/Graphics/Sprite.hpp>
class Ball;
class Tile :  public Entity 
{
public:
	Tile(sf::Vector2f vel, sf::Vector2u pos, const sf::Texture& tex) :Entity(vel, pos, static_cast<std::size_t>(Inkball::Textures::EntityType::TILE)), m_sprite(tex) {
		m_sprite.setPosition((float)pos.x * Inkball::CELL_SIZE, (float)pos.y * Inkball::CELL_SIZE + Inkball::CELL_SIZE); // jump one row 
	}
	virtual ~Tile() {}
	virtual void onContact(Ball&,bool&,bool&,unsigned long&) = 0;
	virtual sf::FloatRect getBounds() {
		return m_sprite.getGlobalBounds();
	}
protected:
	sf::Sprite m_sprite;

};

//hole cretation register holes
#if 0
< 0, 0 ><E, EC, C><velx,vely><spriteDir> track
<0, 0><E, EC, C> spawn
<0, 0><E, EC, C><key1x, key1y, .............>associated keys g
#endif // 0

