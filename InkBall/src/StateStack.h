#pragma once
#include "State.h"
#include <vector>
#include <functional>
#include <map>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include "Identifiers.h"
//This FSM is a modified version  taken from sfml game devlopment book by 
// Jan Haller , Henrik Vogelius Hansson , Artur Moreira

namespace sf
{
	class Event;
	class RenderWindow;
}
class  StateStack : private sf::NonCopyable
{
public:
	enum class Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	explicit StateStack(sf::RenderWindow& window);  //dont need the context for this game since there is a singleton resource class that hols resoources

	template <typename T>
	void	registerState(Inkball::States::Id stateID);

	void	update(sf::Time dt);
	void	draw();
	void	handleEvent(const sf::Event& event);

	void    pushState(Inkball::States::Id stateID);
	void	popState();
	void	clearStates();
	bool	isEmpty() const;


private:
	State::Ptr	createState(Inkball::States::Id stateID);
	void	    applyPendingChanges();


private:
	struct PendingChange
	{
		explicit PendingChange(Action action,Inkball::States::Id stateID = Inkball::States::Id::NONE);

		Action	                s_action;
		Inkball::States::Id  	s_stateID;
	};


private:
	std::vector<State::Ptr>		m_Stack;
	std::vector<PendingChange>	m_PendingList;

	sf::RenderWindow* m_window;
	std::map<Inkball::States::Id, std::function<State::Ptr()>> m_Factories;
};


template <typename T>
void StateStack::registerState(Inkball::States::Id stateID)
{
	m_Factories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, m_window));
	};
}
