#pragma once
#include "Block.h"
#include "Ball.h"
#include "Resource.h"
class Colored : public Block
{
public:
	Colored(const sf::Vector2u &pos,const sf::Texture& tex, Inkball::Textures::Color color,const sf::Vector2f& vel = sf::Vector2f()):Block(vel,pos,tex,color) {}
	virtual ~Colored() {}
	virtual void update(sf::Time dt, Inkball::Action& action) override {};
	virtual void onCollision(Ball& ball)override{
		if (m_color != Inkball::Textures::Color::WHITE && ball.getColor() != m_color) {
			ball.setTexture(Resource::getResourceHandle().getTexture({ static_cast<int>(Inkball::Textures::EntityType::BALL),0,static_cast<int>(m_color) }));
			ball.setColor(m_color);
		}
    }
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override {

		target.draw(m_sprite,states);
	}
};

