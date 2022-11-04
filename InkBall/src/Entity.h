#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

	class Entity : public sf::Drawable
	{
	
	public:
		Entity();
		Entity(sf::Vector2f,sf::Vector2u,std::size_t Id);
		virtual ~Entity(){}
		void setVelocity(sf::Vector2f);
		void setVelocity(float, float);
		void setPosition(sf::Vector2u);
		void setPosition(unsigned,unsigned);
        sf::Vector2u getPosition() const;
	    sf::Vector2f getVelocity() const;
		virtual void update(sf::Time dt);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		std::size_t getID();
	private:
		sf::Vector2f m_velocity;
		sf::Vector2u m_position;  
		std::size_t m_ID; //one of the sub
	};


