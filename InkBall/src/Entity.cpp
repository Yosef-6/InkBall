#include "Entity.h"


Entity::Entity(sf::Vector2f vel, sf::Vector2u pos, std::size_t Id) : m_velocity(vel), m_position(pos), m_ID(Id)
{
}

void Entity::setVelocity(sf::Vector2f vel)
{
	m_velocity.x = vel.x;
	m_velocity.y = vel.y;
}

void Entity::setVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
}

void Entity::setPosition(sf::Vector2u pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
}

void Entity::setPosition(unsigned px,unsigned py)
{
	m_position.x = px;
	m_position.y = py;
}

sf::Vector2u Entity::getPosition() const
{
	return m_position;
}

sf::Vector2f Entity::getVelocity() const
{
	return m_velocity;
}

void Entity::update(sf::Time dt, Inkball::Action& action)
{
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

std::size_t Entity::getID()
{
	return m_ID;
	
}
