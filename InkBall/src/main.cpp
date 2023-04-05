#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "StateStack.h"
#include "Resource.h"
#include "TitleState.h"
#include "GameState.h"
#include "MainMenu.h"
#include "imgui.h"
#include "imgui-SFML.h"  


#include <tuple>

int main()
{
    Resource& res = Resource::getResourceHandle();
    if (res.init()) {
        std::cout << "READY -- " << std::endl;
    }
    sf::RenderWindow window(sf::VideoMode(544,576), "Inkball v1.0 yosef");
    StateStack stack(window);
    stack.registerState<TitleState>(Inkball::States::Id::TITLE);
    stack.registerState<GameState>(Inkball::States::Id::GAME);
    stack.registerState<MainMenu>(Inkball::States::Id::MAIN_MENU);
    ImGui::SFML::Init(window);
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f/60.0f);
    stack.pushState(Inkball::States::Id::TITLE);
    
    
#if 0
    sf::RenderTexture tex;
    Level::loadPreview(tex, 2);
    tex.display();
#endif // 0



    
    
    while (window.isOpen())
    {

        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            sf::Event event;
            while (window.pollEvent(event))
            {
                stack.handleEvent(event);
                if (event.type == sf::Event::Closed)
                    window.close();
            }

           
            stack.update(timePerFrame);
            if (stack.isEmpty())
               window.close();

        }

        window.clear();
        stack.draw();
        window.display();
    }
    res.shutDown(); //avoid global destruction of sfml objects
    ImGui::SFML::Shutdown();
    return 0;
}
