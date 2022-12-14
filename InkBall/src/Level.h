#pragma once
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include "Identifiers.h"

#include "Tile.h"
#include "Block.h"
#include "Ball.h"

class Level 
{
public:
	
	struct ballInfo {
		ballInfo(Ball* ball, float spawn) : s_ball(ball), s_spawnAfter(sf::seconds(spawn)),s_ready(false) {}
		void status(sf::Time dt);
		std::unique_ptr<Ball> s_ball;
		sf::Time s_spawnAfter;
		bool s_ready;
	};
	Level();
	Level(unsigned short level);
	void loadLevel(unsigned short level);
	void loadNext();
	void loadPrev();
	void saveLevel(const std::string& filename);
	void clearLevel();
	std::vector<std::unique_ptr<Entity>> m_levelmap[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE]; // height is used for drawing score and like
	std::vector<ballInfo> m_balls;
	std::vector<std::unique_ptr<sf::Sprite>>m_levelHud;
private:
	template <typename T>
	std::vector<T> parseInp(const std::string& sub, std::size_t digitNo);
	int findNthOccur(const std::string &str, char ch, int N);
	void loadHud();
	//factory functions
	Block* createBlock(const std::vector<std::size_t>& loc,const std::vector<std::size_t>&id,const std::string & sub);
	std::tuple<Ball*,float> createBall(const std::vector<std::size_t>& loc, const std::vector<std::size_t>& id,const std::string& sub);
	Tile*  createTile(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id,const std::string& sub);
	unsigned short m_levelNo;
};
template<typename T>
inline std::vector<T> Level::parseInp(const std::string& sub, std::size_t digitNo)
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
