#pragma once
#include "Tile.h"
#include "Ball.h"
class Hole :  public Tile
{
public:
	using keyType = std::vector<std::tuple<std::size_t, std::size_t>>;
	enum class HolePiece{
		TOP_LEFT,
		TOP_RIGHT,
		BOT_RIGHT,
		BOT_LEFT,
	};
	Hole( sf::Vector2u pos, const sf::Texture& tex, Inkball::Textures::Color color,HolePiece piece,const std::shared_ptr<keyType>& keyloc = std::make_shared<keyType>(),sf::Vector2f vel = sf::Vector2f()):Tile(vel,pos,tex),m_color(color),m_keyLoc(keyloc),m_piece(piece){
     
		if (m_piece == HolePiece::TOP_LEFT)
			m_sprite.setTextureRect(sf::Rect<int>( 0,0,Inkball::CELL_SIZE,Inkball::CELL_SIZE));
		else if(m_piece == HolePiece::TOP_RIGHT)
			m_sprite.setTextureRect(sf::Rect<int>(Inkball::CELL_SIZE, 0, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
		else if (m_piece == HolePiece::BOT_RIGHT)
			m_sprite.setTextureRect(sf::Rect<int>(Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
		else 
			m_sprite.setTextureRect(sf::Rect<int>(0, Inkball::CELL_SIZE, Inkball::CELL_SIZE, Inkball::CELL_SIZE));
		
	}
	virtual ~Hole() {}
	void update(sf::Time dt) override {
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(m_sprite, states);
	}
	void onContact(Ball & ball) override {
		//
		//
		//
	}
private:
	std::shared_ptr<keyType>m_keyLoc;
	Inkball::Textures::Color m_color;
	HolePiece m_piece;

};

