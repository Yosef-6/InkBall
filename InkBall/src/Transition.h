#pragma once
#include "State.h"
class Transition :  public State
{

	Transition(StateStack& stack, sf::RenderWindow* window);
	virtual void  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~Transition() {};
private:
	


};

