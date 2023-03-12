#pragma once
#include "Tile.h"
class spawn :  public Tile
{
public:
	spawn(sf::Vector2u pos, const sf::Texture& tex, sf::Vector2f vel = sf::Vector2f()) :Tile(vel, pos, tex) {

	}
	virtual ~spawn() {}
	void update(sf::Time dt, Inkball::Action& action) override {
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_sprite, states);
	}
	void onContact(Ball& ball,bool& remove) override {
		//
		//
		//
	}
};

