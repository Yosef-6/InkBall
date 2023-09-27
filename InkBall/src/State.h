#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "Identifiers.h"

namespace sf
{
	class RenderWindow;
}
class StateStack;
class State
{
public:
	typedef std::unique_ptr<State> Ptr;

public:
	State(StateStack& stack, sf::RenderWindow* window );
	virtual  ~State();
	virtual bool  draw() = 0;
	virtual bool  update(sf::Time dt) = 0;
	virtual bool  handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(Inkball::States::Id stateID);
	void requestCustomStackPush(State *);
	void requestStackPop();
	void requestStateClear();
	sf::RenderWindow*   m_window;
private:
	StateStack* m_Stack;
};


