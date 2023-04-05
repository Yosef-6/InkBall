#include "Transition.h"



Transition::Transition(StateStack& stack, sf::RenderWindow* window):State(stack,window) {


}

void Transition::draw()
{
}

bool Transition::update(sf::Time dt)
{
    return false;
}

bool Transition::handleEvent(const sf::Event& event)
{
    return false;
}
