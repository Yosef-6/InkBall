#pragma once
#include "State.h"

class LevelEditor :  public State
{
public:
	LevelEditor(StateStack& stack, sf::RenderWindow* window);
	virtual bool  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~LevelEditor();
};

