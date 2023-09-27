#include "MainMenu.h"
#include "Resource.h"
#include "Util.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "GameState.h"



MainMenu::MainMenu(StateStack& stack, sf::RenderWindow* window):State(stack,window),m_pressed(false)
{  
    const Resource& handle = Resource::getResourceHandle();

    m_backg.setTexture(handle.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_FLOOR), 0, 0)));
    m_backg.setPosition(0, 0);

    m_hover.setBuffer(handle.getSoundBuffer(Inkball::Sound::SoundEffects::HOVER));
    m_click.setBuffer(handle.getSoundBuffer(Inkball::Sound::SoundEffects::MENU_CLICK));
    
    m_menu.setFont(handle.getFont(Inkball::Fonts::ARCADE));
    m_menu.setCharacterSize(40U);
    m_menu.setString("Main MENU");
    m_menu.setPosition(Inkball::SCREEN_WIDTH / 2.8f, Inkball::SCREEN_HEIGHT / 5.0f);
    m_menu.setOutlineThickness(2);

    m_items.emplace_back("Play", handle.getFont(Inkball::Fonts::ARCADE),34U );
    m_items.emplace_back("Create  Level", handle.getFont(Inkball::Fonts::ARCADE));
    m_items.emplace_back("Stats", handle.getFont(Inkball::Fonts::ARCADE), 34U);
    m_items.emplace_back("Exit", handle.getFont(Inkball::Fonts::ARCADE), 34U);

    unsigned char i = 0;
    for (sf::Text& text : m_items) {
        text.setOutlineThickness(1.8f);
        text.setPosition(window->getView().getSize().x / 2.8f, window->getView().getSize().y / 1.8f + i * 50);
        i++;
    }
}

bool MainMenu::draw()
{ 
    m_window->draw(m_backg);
    m_window->draw(m_menu);

    unsigned char i = 0;
    for (sf::Text& text : m_items) {

        if (m_currentItemPointer == i) 
            text.setFillColor(sf::Color::Yellow);
        else
            text.setFillColor(sf::Color::White);
        
        m_window->draw(text);
        
        i++;
    }
    return false;
}

bool MainMenu::update(sf::Time dt)
{
    
    
    unsigned int i = 0;
    for (sf::Text& text : m_items) {

        if (text.getGlobalBounds().contains(m_pos)) {
            
            if (m_currentItemPointer != i)
                m_hover.play();

            m_currentItemPointer = i;

           

            if (m_pressed) {
                m_click.play();
                m_pressed = false;
                while (m_click.getStatus() == sf::Sound::Playing);
                requestStackPop();
                switch (i)
                {
                case 0:
                    requestStackPush(Inkball::States::Id::LEVEL_MANAGER);
                    break;
                case 1:
                    requestStackPush(Inkball::States::Id::LEVEL_EDITOR);
                    break;
                case 2:
                    break;
                case 3:
                    break;

                default:
                    break;
                }

            }
        }
        i++;
    }
    return false;
}

bool MainMenu::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved) {
        m_pos.x = event.mouseMove.x;
        m_pos.y = event.mouseMove.y;
    }
    if (event.type == sf::Event::MouseButtonPressed)
        m_pressed = true;
    else
        m_pressed = false;
    return false;
}
