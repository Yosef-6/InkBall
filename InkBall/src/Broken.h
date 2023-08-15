#pragma once
#include "Block.h"
#include <iostream>
class Broken :  public Block
{
public:
	Broken(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color, const sf::Vector2f& vel = sf::Vector2f()):Block(vel, pos, tex, color),m_destroy(false) {}
	virtual ~Broken() {};
	virtual void update(sf::Time dt, Inkball::Action& action) override {
		if (m_destroy) 
			action.mtype = Inkball::ActionType::DELETE_SELF;
	};
	virtual void onCollision(Ball& ball, unsigned long& score)override {
		
		if (m_color == ball.getColor()) 
			m_destroy = true;
			
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_sprite, states);
	}
private:
	bool m_destroy;
};

