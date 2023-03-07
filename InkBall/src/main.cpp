#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "StateStack.h"
#include "Resource.h"
#include "TitleState.h"
#include "GameState.h"

#include "imgui.h"
#include "imgui-SFML.h"  




int main()
{
    Resource& res = Resource::getResourceHandle();
    res.init();
    sf::RenderWindow window(sf::VideoMode(544,576), "Inkball v1.0 yosef");
    StateStack stack(window);
    stack.registerState<TitleState>(Inkball::States::Id::TITLE);
    stack.registerState<GameState>(Inkball::States::Id::GAME);
    ImGui::SFML::Init(window);
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f/60.0f);
    stack.pushState(Inkball::States::Id::TITLE);
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
/*

int main()
{

   

    std::vector<sf::VertexArray> vertices; // vector in wich all vertexArrays (lines, rectangles) will be stored

    enum class states { Line, Rectangle, Circle }; // current drawing mode
    states current_state = states::Line;

    // Lines
    vertices.push_back(sf::VertexArray()); // 1st line
    vertices[0].setPrimitiveType(sf::LinesStrip); // PrimitiveType of the 1st line: see https://www.sfml-dev.org/tutorials/2.4/graphics-vertex-array.php
    bool L_locked = false; // (only when in line mode) 


    // Rectangles
    bool R_first_MB_released = false; // (only when in rectangle mode)
    sf::Vector2f R_first; // position of the 1st mouse button released
    sf::Vector2f R_second; // position of the 2nd mouse button released
    bool R_locked = false;

   

    //sf::RectangleShape s;
    //std::cout << "RectangleShape: " << sizeof(s) << ", VertexArray: " << sizeof(construction) << std::endl; std::cin.get(); std::cin.get(); // RectangleShape vs VertexArray


    sf::Color curr_col = sf::Color::Black; // color 
    sf::Vector2i last_Mouse_pos(0, 0);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "µPaint", sf::Style::Close, sf::ContextSettings(0, 0, 0)); // where everythings drawn to
    window.setFramerateLimit(60);

    sf::Vector2i Border_Offset(-5, -25); // See (peculiarities in the 1st part: https://steemit.com/steemstem/@numbo/c-sfml-paint-a-simple-drawing-program )

    while (window.isOpen())
    {

        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::KeyPressed) // closing window
                if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
            if (event.type == sf::Event::Closed) // closing window
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
               
                    L_locked = true;
  

            }

            if (event.type == sf::Event::MouseButtonReleased)
            {

                if (current_state == states::Line)
                {
                    // Add a new line
                    vertices.push_back(sf::VertexArray());
                    vertices.back().setPrimitiveType(sf::LinesStrip);

                    L_locked = false; // Reset line
                }
       

            }
        } // End Events


        // Construction of a line
        if (L_locked)
        {
            if (last_Mouse_pos != sf::Mouse::getPosition()) // Only add, when the mouse has moved (save memory)
            {
                //.append(Position, Farbe) : .append(MousePos - WindowPos + MouseOffset, curr_col)
                vertices[vertices.size() - 1].append(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition().x - window.getPosition().x + Border_Offset.x, sf::Mouse::getPosition().y - window.getPosition().y + Border_Offset.y), curr_col));

                last_Mouse_pos = sf::Mouse::getPosition();
            }
        }

      

        //curr_col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);


        //std::cout << "vertices in line " << lines_number << ": " << vertices[lines_number].getVertexCount() << std::endl;

        std::cout << "V1: " << R_first.x << " | " << R_first.y << " V2: " << R_second.x << " | " << "vertices-vec: " << vertices.size() << std::endl;

        window.clear(sf::Color::White); // delete the current scene with a specific color

        // draw everything
        for (int i = 0; i < vertices.size(); i++)
        {
            window.draw(vertices[i]);
        }

        

        window.display();
    }


    return 0;
}


*/