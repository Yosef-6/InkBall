#include "MessageState.h"
#include "Resource.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"



MessageState::MessageState(StateStack& stack, sf::RenderWindow* window, Inkball::Mode mode):State(stack,window),m_mode(mode)
{
	const Resource& res = Resource::getResourceHandle();
	m_backg.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::PAUSE), 0, 0)));

	m_message.setFont(res.getFont(Inkball::Fonts::ARCADE));
	m_message.setCharacterSize(40U);
	m_message.setOutlineThickness(2);
	if (m_mode == Inkball::Mode::PAUSED) {
		m_message.setPosition(Inkball::SCREEN_WIDTH / 2.7f, Inkball::SCREEN_HEIGHT / 3.0f);
		m_message.setString("PAUSED");
	}
	else if (m_mode == Inkball::Mode::GAME_WON) {
		m_message.setPosition(Inkball::SCREEN_WIDTH / 2.4f, Inkball::SCREEN_HEIGHT / 3.0f);
		m_message.setString("Level Passed");
	}
   else
	 m_message.setString("GAME OVER");
}

bool MessageState::draw()
{
	
	m_window->draw(m_backg);
	m_window->draw(m_message);
	
	return true;
}

bool MessageState::update(sf::Time dt)
{

	return false;
}

bool MessageState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && clock.getElapsedTime().asSeconds() > 0.5f) {//done to avoid too many fixed updates on game state after it has been interrupted by pressing escape two times quickly

		if (event.key.code == sf::Keyboard::Escape) 
           requestStackPop();
		else if (event.key.code == sf::Keyboard::M) {
			requestStackPop();
			requestStackPop();
			requestStackPush(Inkball::States::Id::MAIN_MENU);
		}
		else if(event.key.code == sf::Keyboard::L) {
			requestStackPop();
			requestStackPop();
			requestStackPush(Inkball::States::Id::LEVEL_MANAGER);
		}
		else if (m_mode == Inkball::Mode::GAME_WON && event.key.code == sf::Keyboard::N) {
			requestStackPop();
			requestStackPop();
			requestStackPush(Inkball::States::Id::LEVEL_MANAGER);
		}

	}
	return false;
}
