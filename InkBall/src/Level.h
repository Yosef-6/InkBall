#pragma once
#include <string>
#include <memory>
#include <vector>
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
		ballInfo(Ball* ball, float spawn) : s_ball(ball), s_spawnAfter(sf::seconds(spawn)),s_ready(false),remove(false) {}
		void status(sf::Time dt);
		std::unique_ptr<Ball> s_ball;
		sf::Time s_spawnAfter;
		bool s_ready;
		bool remove;
	};
	Level();
	Level(unsigned short level);
	bool loadLevel(unsigned short level);
	static void loadPreview(sf::RenderTexture&,unsigned short);
	void saveLevel(const std::string& filename);
	void clearLevel();
	void updateLevel(sf::Time dt);
	std::vector<std::unique_ptr<Entity>> m_levelmap[Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE][Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE]; // height is used for drawing score and like
	std::vector<ballInfo> m_balls;
	std::vector<std::unique_ptr<sf::Sprite>>m_levelHud;

private:
	template <typename T>
	static std::vector<T> parseInp(const std::string& sub, std::size_t digitNo);
	static int findNthOccur(const std::string &str, char ch, int N);
	void loadHud();
	//factory functions
	Block* createBlock(const std::vector<std::size_t>& loc,const std::vector<std::size_t>&id,const std::string & sub);
	std::tuple<Ball*,float> createBall(const std::vector<std::size_t>& loc, const std::vector<std::size_t>& id,const std::string& sub);
	Tile*  createTile(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id,const std::string& sub);
	unsigned short m_levelNo;
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
