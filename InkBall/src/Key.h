#pragma once
#include "Block.h"
#include "Animate.h"
class Key :  public Block
{
public:
	Key(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color,const sf::Vector2f& vel = sf::Vector2f()) :Block(vel, pos, tex, color),m_animationObj(&m_sprite,5,false,100),m_unlocked(false){}
	virtual ~Key() {}
	virtual void update(sf::Time dt) override {
		if (m_unlocked) {
			m_animationObj.updateState(dt.asSeconds());

		}
	};
	virtual void onCollision(Ball& ball)override {

	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(m_sprite, states);
	}
private:
	bool m_unlocked;
	Animate m_animationObj;
};

