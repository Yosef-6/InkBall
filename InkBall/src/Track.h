#pragma once
#include "Tile.h"
class Track :  public Tile
{
public:
	Track(sf::Vector2u pos, const sf::Texture& tex, Inkball::Direction spriteDir,sf::Vector2f vel = sf::Vector2f()) :Tile(vel, pos, tex),m_spriteDir(spriteDir) {
		auto l_pos = m_sprite.getPosition();
		m_sprite.setOrigin(Inkball::CELL_SIZE / 2.0f, Inkball::CELL_SIZE / 2.0f);
		m_sprite.rotate(static_cast<int>(spriteDir) * -90.0f);
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
private:
	Inkball::Direction m_spriteDir;
};

