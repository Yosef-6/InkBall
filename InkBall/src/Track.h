#pragma once
#include "Tile.h"
class Track :  public Tile
{
public:
	Track(sf::Vector2u pos, const sf::Texture& tex, sf::Vector2f vel = sf::Vector2f()) :Tile(vel, pos, tex) {
	
	}
	virtual ~Track() {}
	void update(sf::Time dt) override {
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_sprite, states);
	}
	void onContact(Ball& ball) override {
		//
		//
		//
	}
};

