#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <sstream>
#include "Identifiers.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "Tile.h"
#include "Block.h"
#include "Ball.h"

class Level 
{
public:
	
	struct ballInfo {
		ballInfo(Ball* ball, float spawn, bool update) : s_ball(ball), m_remove(false), m_match(true),m_supdate(update),m_threadCreated(false) {
		 
			if (spawn == 0.0f)
				m_spawnAfter = sf::Time::Zero;
			else
				m_spawnAfter = sf::seconds(spawn);
		
		}
		void status(sf::Time dt);
		std::unique_ptr<Ball> s_ball;
		sf::Time m_spawnAfter;
		bool m_remove, m_match, m_supdate ,m_threadCreated;
	};
	Level();
	bool loadLevel(std::string&);
	static void loadPreview(sf::RenderTexture&,const char*);
	void saveLevel(const std::string& filename);
	void clearLevel();
	void updateLevel(sf::Time dt);
	std::vector<std::unique_ptr<Entity>> m_levelmap[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE]; // height is used for drawing score and like
	std::vector<ballInfo> m_balls;
	std::vector<sf::Sprite>m_levelHud;
	std::vector<sf::Text>m_levelStr;
	std::vector<sf::Sprite>m_ballsDisplay;

private:
	bool m_prevLoaded = false;
	template <typename T>
	static std::vector<T> parseInp(const std::string& sub, std::size_t digitNo);
	static int findNthOccur(const std::string &str, char ch, int N);
	void loadDisplay(); //dynamic display lvl
	void loadHud();//static part of level
	//factory functions
	Block* createBlock(const std::vector<std::size_t>& loc,const std::vector<std::size_t>&id,const std::string & sub);
	std::tuple<Ball*,float,bool> createBall(const std::vector<std::size_t>& loc, const std::vector<std::size_t>& id,const std::string& sub);
	Tile*  createTile(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id,const std::string& sub);
};
template<typename T>
static std::vector<T> Level::parseInp(const std::string& sub, std::size_t digitNo)
{
	std::stringstream ss;
	std::size_t left = 0;
	std::vector<T> vec;
	T&& val = (T)(0.0);
	vec.reserve(digitNo);
	for (std::size_t i = 0; i < digitNo; i++) {
		ss << sub.substr(left, sub.find(",", left) - left);
		ss >> val;
		vec.push_back(val);
		left = sub.find(",", left) + 1;
		ss.clear();
		ss.str(std::string());
	}
	return vec;
}
