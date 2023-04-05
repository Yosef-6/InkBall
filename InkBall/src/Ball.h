#pragma once
#include "Entity.h"
#include "Identifiers.h"

class Ball : public Entity
{
 public:
	Ball(sf::Vector2f vel,sf::Vector2u pos,Inkball::Textures::Color color,const sf::Texture & tex):Entity(vel,pos,static_cast<std::size_t>(Inkball::Textures::EntityType::BALL)),m_color(color),m_sprite(tex) {
		m_sprite.setPosition((float)pos.x * Inkball::CELL_SIZE + 2.f, (float)pos.y * Inkball::CELL_SIZE + Inkball::CELL_SIZE);
		m_sprite.scale(0.8f,0.8f);
		setCellPos();
	}
	virtual ~Ball() {}
	virtual void update(sf::Time dt)  {
		m_sprite.move(getVelocity() * dt.asSeconds());
		setCellPos();
	}
	void resetScale() {
		m_sprite.setScale(0.8f,0.8f);
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
	void setColor(Inkball::Textures::Color newColor) {
		m_color = newColor;
	}
	
private:
	Inkball::Textures::Color m_color;
	sf::Sprite m_sprite;
};

