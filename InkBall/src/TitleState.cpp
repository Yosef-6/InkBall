#include "TitleState.h"
#include "Resource.h"
#include "Util.h"
#include <tuple>
#include <SFML/Graphics/RenderWindow.hpp>
TitleState::TitleState(StateStack& stack, sf::RenderWindow* window) :State(stack, window), m_logo(), m_text(), m_showText(true), m_textBlinkTime(sf::Time::Zero), m_Inkball(),m_backg(),m_message()
{
    const Resource& res = Resource::getResourceHandle();
    m_logo.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::LOGO), 0, 0)));
    m_backg.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::S_FLOOR), 0, 0)));
    m_text.setFont(res.getFont(Inkball::Fonts::TEXT1));
    m_Inkball.setFont(res.getFont(Inkball::Fonts::TEXT1));
    m_message.setFont(res.getFont(Inkball::Fonts::TEXT1));
    m_message.setOutlineThickness(2);
    m_Inkball.setOutlineThickness(2);
    m_text.setOutlineThickness(2);
    m_logo.scale(0.5f,0.5f);
    m_Inkball.setString("Inkball");
    m_text.setString("Press any key to start");
    m_message.setString("Inkball clone written by YosefSirak 2022");
    m_message.setScale(0.5f,0.5f);
    m_backg.setPosition(0, 0);
    Util::centerOrigin(m_logo);
    Util::centerOrigin(m_text);
    Util::centerOrigin(m_Inkball);
    Util::centerOrigin(m_message);
    m_logo.setPosition(Inkball::SCREEN_WIDTH/3.0f,Inkball::SCREEN_HEIGHT/3.0f);
    m_Inkball.setPosition(Inkball::SCREEN_WIDTH / 2.0f + 50.0f, Inkball::SCREEN_HEIGHT / 3.0f);
    m_text.setPosition(window->getView().getSize().x/2.0f , window->getView().getSize().y/1.50f);
    m_message.setPosition(window->getView().getSize().x / 2.0f, window->getView().getSize().y / 1.15f);

}

void TitleState::draw()
{
    m_window->draw(m_backg);
    m_window->draw(m_logo);
    m_window->draw(m_Inkball);
    m_window->draw(m_message);
    if (m_showText)
        m_window->draw(m_text);
}

bool TitleState::update(sf::Time dt)
{
    m_textBlinkTime += dt;
    if (m_textBlinkTime > sf::seconds(0.5f))
    {
        m_textBlinkTime = sf::Time::Zero;
        m_showText = !m_showText;
    }
    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        requestStackPop();
        requestStackPush(Inkball::States::Id::MAIN_MENU);  
    }
    return true;
}

TitleState::~TitleState()
{
}
