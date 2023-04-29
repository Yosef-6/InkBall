#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack, sf::RenderWindow* window):m_window(window),m_Stack(&stack)
{

}
State::~State()
{
}

void State::requestStackPush(Inkball::States::Id stateID)
{ 
	m_Stack->pushState(stateID);
}

void State::requestCustomStackPush(State* ptr)
{
	m_Stack->pushState(ptr);
}

void State::requestStackPop()
{
	m_Stack->popState();
}

void State::requestStateClear()
{
	m_Stack->clearStates();
}
