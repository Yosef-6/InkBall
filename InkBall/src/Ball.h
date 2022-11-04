#pragma once
#include "Entity.h"
#include "Identifiers.h"

class Ball : public Entity
{
 public:
	Ball(sf::Vector2f vel,sf::Vector2u pos,Inkball::Textures::Color color,const sf::Texture & tex):Entity(vel,pos,static_cast<std::size_t>(Inkball::Textures::EntityType::BALL)),m_color(color),m_sprite(tex) {
		m_sprite.setPosition((float)pos.x * Inkball::CELL_SIZE, (float)pos.y * Inkball::CELL_SIZE + Inkball::CELL_SIZE);
		setCellPos();
	}
	virtual ~Ball() {}
	virtual void update(sf::Time dt) override {
		m_sprite.move(getVelocity() * dt.asSeconds());
		setCellPos();
	}
private:
	void setCellPos() {
		setPosition( size_t(m_sprite.getPosition().x / Inkball::CELL_SIZE), size_t(m_sprite.getPosition().y  / Inkball::CELL_SIZE )); // cellpostions
	}
public:
	sf::Sprite& getSprite() {
		return m_sprite;
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_sprite, states);
	}
	void setTexture(const sf::Texture& texture) {
		m_sprite.setTexture(texture);
	}
	Inkball::Textures::Color getColor() {
		return m_color;
	}
	
private:
	Inkball::Textures::Color m_color;
	sf::Sprite m_sprite;
};

