#pragma once
#include "Block.h"
#include "Collision.h"
class Directional : public Block
{
public:
	Directional(const sf::Vector2u& pos, const sf::Texture& tex, Inkball::Textures::Color color, Inkball::Direction spriteDir, Inkball::Direction logicDir,Inkball::Textures::BlockType type,const sf::Vector2f& vel = sf::Vector2f()):Block(vel, pos, tex,color),m_spriteDirection(spriteDir),m_logicDirection(logicDir),m_type(type){
		auto l_pos = m_sprite.getPosition();
		m_sprite.setOrigin(Inkball::CELL_SIZE / 2.0f, Inkball::CELL_SIZE / 2.0f);
		m_sprite.rotate(static_cast<int>(spriteDir)*-90.0f);
		m_sprite.setPosition(l_pos.x + Inkball::CELL_SIZE / 2.0f, l_pos.y + Inkball::CELL_SIZE / 2.0f);


		sf::FloatRect gBounds = m_sprite.getGlobalBounds();

		if (m_type == Inkball::Textures::BlockType::DIRECTIONAL2)
			switch (m_spriteDirection) {

			case Inkball::Direction::DOWN:
			case Inkball::Direction::UP:
				m_bounds = sf::FloatRect(gBounds.left + 2, gBounds.top + 11, 30, 11);
				break;
			case Inkball::Direction::RIGHT:
			case Inkball::Direction::LEFT:
				m_bounds = sf::FloatRect(gBounds.left + 11, gBounds.top + 2, 11, 30);
				break;
			}
		else
			switch (m_spriteDirection) {

			case Inkball::Direction::DOWN:
				m_bounds = sf::FloatRect(gBounds.left + 2, gBounds.top + 21, 30, 11);
				break;
			case Inkball::Direction::UP:
				m_bounds = sf::FloatRect(gBounds.left + 2, gBounds.top + 1, 30, 11);
				break;
			case Inkball::Direction::RIGHT:
				m_bounds = sf::FloatRect(gBounds.left + 21, gBounds.top + 2, 11, 30);
				break;
			case Inkball::Direction::LEFT:
				m_bounds = sf::FloatRect(gBounds.left + 1, gBounds.top + 2, 11, 30);
				break;
			}


	}
	virtual void update(sf::Time dt, Inkball::Action& action) override {};
	virtual void onCollision(Ball& ball, unsigned long& score)override {
		
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(m_sprite, states);
	}
	virtual bool allowCollision(sf::Vector2f vel, Inkball::Textures::Color color) override {

		sf::Vector2f dirs[4] = { // this is the same orfering as enum Inkball::Direction
			{0.0f, -1.0f},
			{-1.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 0.0f},
		};

		double dotProd = Collsion::dot(vel / Collsion::length(vel), dirs[(size_t)m_logicDirection-1]);
		 
		if (m_color == Inkball::Textures::Color::WHITE && dotProd > 0 || m_color == color && dotProd > 0)
			return false;

		return true;
	}
	virtual sf::FloatRect getBounds() override {
		/// 11px    width 11 px      28 pixel height
		//to do select the padddle from whole sprite
		return m_bounds;
	}
private:
	Inkball::Direction m_spriteDirection;  // allowed direction this block passes the ball for dir1 type of block
	Inkball::Direction m_logicDirection;
	Inkball::Textures::BlockType m_type;
	sf::FloatRect m_bounds;

};

