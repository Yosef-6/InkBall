#pragma once
#include "Block.h"
#include "Animate.h"
class Timer : public Block
{
public:
	Timer(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color,float tick ,const sf::Vector2f& vel = sf::Vector2f()) :Block(vel, pos, tex, color), m_animationObj(&m_sprite, 5,true,tick),m_frameNo(1){} // additional tick speed
	virtual ~Timer() {}
	virtual void update(sf::Time dt, Inkball::Action& action) override {
		   // std::cout << m_skipCollision << std::endl;
			m_frameNo  =   m_animationObj.updateState(dt.asSeconds());
			
	};
	virtual void onCollision(Ball& ball)override {
		
		
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(m_sprite, states);
	}
	virtual bool allowCollision(sf::Vector2f vel, Inkball::Textures::Color color) override {
		if (m_frameNo == 0 )
			return false;
		
		return true;
	}
private:
	Animate m_animationObj;
	unsigned short m_frameNo;

};

