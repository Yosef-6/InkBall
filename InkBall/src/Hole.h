#pragma once
#include "Tile.h"
#include "Ball.h"
#include <iostream>
#include "Collision.h"
class Hole :  public Tile
{
public:
	using keyType = std::vector<std::tuple<std::size_t, std::size_t>>;
	enum class HolePiece{
		TOP_LEFT = 0,
		TOP_RIGHT,
		BOT_RIGHT,
		BOT_LEFT,
	};
	Hole( sf::Vector2u pos, const sf::Texture& tex, Inkball::Textures::Color color,HolePiece piece,const std::shared_ptr<keyType>& keyloc = std::make_shared<keyType>(),sf::Vector2f vel = sf::Vector2f()):Tile(vel,pos,tex),m_color(color),m_keyLoc(keyloc){
     

		 // hole centrs can be calculate before the hole is split up into 4 parts
		if (piece == HolePiece::TOP_LEFT) {
			m_sprite.setTextureRect(sf::Rect<int>(0, 0, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
			sf::FloatRect bounds = m_sprite.getGlobalBounds();
			m_holeCenter = sf::Vector2f(bounds.left + bounds.width,bounds.top + bounds.height);
		}
		else if (piece == HolePiece::TOP_RIGHT) {
			m_sprite.setTextureRect(sf::Rect<int>(Inkball::CELL_SIZE, 0, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
			sf::FloatRect bounds = m_sprite.getGlobalBounds();
			m_holeCenter = sf::Vector2f(bounds.left, bounds.top + bounds.height);
		}
		else if (piece == HolePiece::BOT_RIGHT) {
			m_sprite.setTextureRect(sf::Rect<int>(Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
			sf::FloatRect bounds = m_sprite.getGlobalBounds();
			m_holeCenter = sf::Vector2f(bounds.left, bounds.top);
		}
		else {
			m_sprite.setTextureRect(sf::Rect<int>(0, Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
			sf::FloatRect bounds = m_sprite.getGlobalBounds();
			m_holeCenter = sf::Vector2f(bounds.left + bounds.width,bounds.top );
		}
		
	}
	virtual ~Hole() {}
	void update(sf::Time dt, Inkball::Action& action) override {
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_sprite, states);
	}
	void onContact(Ball & ball,bool& remove) override {
	
	sf::FloatRect ballBounds  = ball.getSprite().getGlobalBounds();
	sf::Vector2f ballCenter   = sf::Vector2f(ballBounds.left + ballBounds.width/2.0f, ballBounds.top + ballBounds.height / 2.0f);

	auto inScale = ball.getSprite().getScale();

	float dist = Collsion::length(m_holeCenter - ballCenter);
		if ( dist < radiusOuter ) {
		
			ball.getSprite().setScale(inScale - sf::Vector2f(0.07, 0.07));
			ball.setVelocity(ball.getVelocity() + ( (m_holeCenter - ballCenter) / Collsion::length(m_holeCenter - ballCenter) ) * 8.0f);
	  
			if (dist < innerRadius) // ball is assumed to enter the hole
				remove = true;

		}

	}
private:
	std::shared_ptr<keyType>m_keyLoc;
	Inkball::Textures::Color m_color;
	const float radiusOuter = 22.0f;
	const float innerRadius = 10.0f; // pixels
	sf::Vector2f m_holeCenter;
};

