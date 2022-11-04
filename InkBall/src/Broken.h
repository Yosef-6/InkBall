#pragma once
#include "Block.h"
class Broken :  public Block
{
public:
	Broken(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color, const sf::Vector2f& vel = sf::Vector2f()):Block(vel, pos, tex, color) {}
	virtual ~Broken() {};
	virtual void update(sf::Time dt) override {};
	virtual void onCollision(Ball& ball)override {
		//handle collision 
		
		//destroy this object 
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_sprite, states);
	}
};

