#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "StateStack.h"
#include "Resource.h"
#include "TitleState.h"
#include "GameState.h"
#include "LevelManager.h"
#include "MainMenu.h"
#include "LevelEditor.h"





int main()
{
    Resource& res = Resource::getResourceHandle();
    if (res.init()) {
        std::cout << "READY -- " << std::endl;
    }
    sf::RenderWindow window(sf::VideoMode(544, 576), "Inkball v1.0 yosef", sf::Style::Close);
    StateStack stack(window);
    stack.registerState<LevelEditor>(Inkball::States::Id::LEVEL_EDITOR);
    stack.registerState<TitleState>(Inkball::States::Id::TITLE);
    stack.registerState<MainMenu>(Inkball::States::Id::MAIN_MENU);
    stack.registerState<LevelManager>(Inkball::States::Id::LEVEL_MANAGER);
    stack.pushState(Inkball::States::Id::TITLE);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
   


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

    return 0;
}
#if 0
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "StateStack.h"
#include "Resource.h"
#include "TitleState.h"
#include "GameState.h"
#include "LevelManager.h"
#include "MainMenu.h"
#include "LevelEditor.h"




#include <tuple>

int main()
{
    Resource& res = Resource::getResourceHandle();
    if (res.init()) {
        std::cout << "READY -- " << std::endl;
    }
    sf::RenderWindow window(sf::VideoMode(544, 576), "Inkball v1.0 yosef", sf::Style::Close);



#if 0
    sf::RenderTexture tex;
    Level::loadPreview(tex, 2);
    tex.display();
#endif // 0

    sf::Clock clock;
    while (window.isOpen())
    {

        sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            stack.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();

        }
        stack.update(dt);

        if (stack.isEmpty())
            window.close();



        window.clear();

        stack.draw();

        window.display();
    }
    res.shutDown(); //avoid global destruction of sfml objects
    return 0;
}

#endif // 0

