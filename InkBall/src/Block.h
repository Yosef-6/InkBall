#pragma once
#include "Entity.h"
#include "Identifiers.h"
#include <SFML/Graphics/Sprite.hpp>
class Ball;
class Block : public Entity
{
public:
	//Block();
	virtual ~Block() {}
	Block(sf::Vector2f vel, sf::Vector2u pos,const sf::Texture & tex, Inkball::Textures::Color color) :Entity(vel, pos, static_cast<std::size_t>(Inkball::Textures::EntityType::BLOCK)),m_sprite(tex,sf::IntRect(0,0,Inkball::CELL_SIZE, Inkball::CELL_SIZE)) ,m_color(color){
		
		m_sprite.setPosition((float)pos.x*Inkball::CELL_SIZE,(float)pos.y * Inkball::CELL_SIZE + Inkball::CELL_SIZE); // jump one row for the score and other 
	}
	virtual void onCollision(Ball&,unsigned long&) = 0;
	virtual sf::FloatRect getBounds() {
		return m_sprite.getGlobalBounds();
	}
	virtual bool allowCollision(sf::Vector2f vel,Inkball::Textures::Color color) {
		return true;
	}
protected:
	sf::Sprite m_sprite;
	Inkball::Textures::Color m_color;
};

