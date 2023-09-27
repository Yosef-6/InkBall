#pragma once
#include "Tile.h"
class Track :  public Tile
{
public:
	Track(sf::Vector2u pos, const sf::Texture& tex,sf::Vector2f vel = sf::Vector2f()) :Tile(vel, pos, tex) {
		auto l_pos = m_sprite.getPosition();
		m_sprite.setOrigin(Inkball::CELL_SIZE / 2.0f, Inkball::CELL_SIZE / 2.0f);
		if (vel.x == 0)
			vel.x = 0.000001f;
		m_sprite.rotate(180.0f / 3.14159265 * atan((vel.y/vel.x )));
		std::cout << 180.0f / 3.14159265 * atan((vel.y / vel.x)) << vel.x<< vel.y << std::endl;
		m_sprite.setPosition(l_pos.x + Inkball::CELL_SIZE / 2.0f, l_pos.y + Inkball::CELL_SIZE / 2.0f);
	}
	virtual ~Track() {}
	void update(sf::Time dt, Inkball::Action& action) override {
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_sprite, states);
	}
	void onContact(Ball& ball,bool& remove , bool& matches,unsigned long& score) override {
		//std::cout << " " << getVelocity().x << "   :   " << getVelocity().y << std::endl;
		ball.setVelocity(ball.getVelocity() + getVelocity());
	}
};

