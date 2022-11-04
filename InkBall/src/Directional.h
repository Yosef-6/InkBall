#pragma once
#include "Block.h"
class Directional : public Block
{
public:
	Directional(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color, Inkball::Direction spriteDir, Inkball::Direction logicDir,Inkball::Textures::BlockType type,const sf::Vector2f& vel = sf::Vector2f()):Block(vel, pos, tex,color),m_spriteDirection(spriteDir),m_logicDirection(logicDir),m_type(type){
		auto l_pos = m_sprite.getPosition();
		m_sprite.rotate(static_cast<int>(spriteDir)*-90.0f);
		m_sprite.setOrigin(Inkball::CELL_SIZE/2.0f, Inkball::CELL_SIZE / 2.0f);
		m_sprite.setPosition(l_pos.x + Inkball::CELL_SIZE / 2.0f, l_pos.y + Inkball::CELL_SIZE / 2.0f);
	}
	virtual void update(sf::Time dt) override {};
	virtual void onCollision(Ball& ball)override {
		// resolve collsion based on direction and color
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(m_sprite, states);
	}
	virtual sf::FloatRect getBounds() override {
		///
		//to do select the padddle from whole sprite

	
		return sf::FloatRect();
	}
private:
	Inkball::Direction m_spriteDirection;  // allowed direction this block passes the ball for dir1 type of block
	Inkball::Direction m_logicDirection;
	Inkball::Textures::BlockType m_type;

};

