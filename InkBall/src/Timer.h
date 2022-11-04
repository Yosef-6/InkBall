#pragma once
#include "Block.h"
#include "Animate.h"
class Timer : public Block
{
public:
	Timer(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color, const sf::Vector2f& vel = sf::Vector2f()) :Block(vel, pos, tex, color), m_animationObj(&m_sprite, 5,true,0.75f){} // additional tick speed
	virtual ~Timer() {}
	virtual void update(sf::Time dt) override {
		std::cout << "animation" << std::endl;
			m_animationObj.updateState(dt.asSeconds());
	};
	virtual void onCollision(Ball& ball)override {
		//
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(m_sprite, states);
	}
private:
	Animate m_animationObj;


};

