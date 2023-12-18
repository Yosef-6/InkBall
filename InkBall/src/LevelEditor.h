#pragma once
#include "State.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "Entity.h"
#include "SFML/Audio/Sound.hpp"
#include <unordered_map>
#include <vector>
#include <string>


class LevelEditor :  public State
{
public:
	LevelEditor(StateStack& stack, sf::RenderWindow* window);
	virtual bool  draw() override;
	virtual bool  update(sf::Time dt) override;
	virtual bool  handleEvent(const sf::Event& event) override;
	virtual ~LevelEditor();
private:
	struct Item  {
		sf::Sprite m_itemSprite;
		std::tuple<size_t, size_t> m_cellPos{0,0};
		std::tuple<size_t, size_t, size_t> m_entityType{0,0,0};
		std::vector<std::vector<size_t>> m_otherProps;
		bool m_lastItem;
	}; 
private:
	void setSelector(const std::string&);
	std::vector<std::pair<int, int>> calculatePlacingCells();
private:
	sf::RenderTexture m_overlay;
	sf::Sprite m_backg , m_hud , m_items , m_select , m_overlaySprite , m_selectedItemSprite;
	std::string m_selectedItemCode;
	std::vector<Item> m_board[17][17];
	std::unordered_map<std::string, sf::IntRect*> m_itemsMapSize; // clean afterward
	std::unordered_map<std::string, std::tuple<size_t,size_t,size_t>> m_itemsMapEntityCode; 
	//std::unordered_map<std::string, std::tuple<size_t, size_t, size_t>> m_itemsMapActions; 
	sf::Sound m_hover, m_click;
	bool  m_itemMove = false, m_placed = false;
	
}; 

