#include "Level.h"
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include "Hole.h"
#include "Track.h"
#include "spawn.h"
#include "Key.h"
#include "Timer.h"
#include "Broken.h"
#include "Colored.h"  
#include "Directional.h"



//
//
//
//
// 


Level::Level(){

}
bool Level::loadLevel(std::string &lvl) // exceptions are a handled in game state
{
	std::stringstream buffer;
	std::string entity;
	std::ifstream file("res/levels/" + lvl);
	if (!file.is_open()) 
		throw std::runtime_error("Level::loadLevel - Failed to load " + buffer.str());
	buffer.str(std::string());
	while (std::getline(file, entity)) {
		auto pos    = parseInp<std::size_t>(entity.substr(1,findNthOccur(entity,'>',1) - 1), 2);
		std::size_t cellx = pos[0];
		std::size_t celly = pos[1];
		if(cellx >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE || celly >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE)
			throw std::runtime_error("Level::loadLevel - unknown format ");
	//	std::cout<<cellx<<"   "<<celly<<std::endl;
	    //E entity Ec entity class C 
		int left =  findNthOccur(entity, '<', 2) + 1; //6
		auto entityInfo = parseInp<std::size_t>(entity.substr(left, findNthOccur(entity, '>', 2) - left), 3);
		if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::BLOCK))
			m_levelmap[cellx][celly].emplace_back(createBlock(pos, entityInfo, entity));
		else if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::TILE))
			m_levelmap[cellx][celly].emplace_back(createTile(pos, entityInfo, entity));
		else if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::BALL)) {
			//ball is special object is not fixd inside the table (moves over the table)
			auto [ball, spawn, update] = createBall(pos, entityInfo, entity);
			m_balls.emplace_back( ball, spawn, update );
		}
		else
		throw std::runtime_error("Level::loadLevel - unknown format ");
	}
	file.close();

	//load backg if it has not been loaded previously
	if(m_prevLoaded == false)
	loadHud();

	loadDisplay();
}

void Level::loadPreview(sf::RenderTexture& base, const char * path)
{

	const Resource& res = Resource::getResourceHandle();

	if (!base.create(Inkball::SCREEN_WIDTH, Inkball::SCREEN_WIDTH)) // 
		return;

	std::stringstream buffer;
	std::string entity;
	std::ifstream file("res/levels/" + std::string(path));
	if (!file.is_open()) {

		base.draw(sf::Sprite(res.getTexture(std::tuple(static_cast<int>(Inkball::Textures::OtherType::ERROR_PREVIEW), 0, 0))));
		return;
	}
	else
		base.draw(sf::Sprite(res.getTexture(std::tuple(static_cast<int>(Inkball::Textures::OtherType::FLOOR), 0, 0))));

	try {
		while (std::getline(file, entity)) {

			sf::Sprite tempBuffer;

			auto pos = parseInp<std::size_t>(entity.substr(1, findNthOccur(entity, '>', 1) - 1), 2);
			std::size_t cellx = pos[0];
			std::size_t celly = pos[1];
			if (cellx >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE || celly >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE)
				throw std::runtime_error("Level::loadPreview - unknown format ");

			int left = findNthOccur(entity, '<', 2) + 1; //6
			auto entityInfo = parseInp<std::size_t>(entity.substr(left, findNthOccur(entity, '>', 2) - left), 3);
			if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::BLOCK)) {

				if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::COLORED) || entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::BROKEN)) {
					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setTexture(res.getTexture(std::tuple<int, int, int>(entityInfo[0], entityInfo[1], entityInfo[2])));
					base.draw(tempBuffer);
				}

				else if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::DIRECTIONAL) || entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::DIRECTIONAL2)) {
					//direction 0 spriteDir 1 logicDir
					int left1 = findNthOccur(entity, '>', 2);
					if (left1 == -1)
						throw std::runtime_error("Level::loadPreview - failed due to unknown format");
					auto direction = parseInp<std::size_t>(entity.substr(left1 + 2, 3), 2);
					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setOrigin(Inkball::CELL_SIZE / 2.0f, Inkball::CELL_SIZE / 2.0f);
					tempBuffer.rotate(direction[0] * -90.0f);
					tempBuffer.setPosition(pos[0] + Inkball::CELL_SIZE / 2.0f, pos[1] + Inkball::CELL_SIZE / 2.0f);
					base.draw(tempBuffer);
				}
				else if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::KEY) || entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::TIMER)) {
					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setTexture(res.getTexture(std::tuple<int, int, int>(entityInfo[0], entityInfo[1], entityInfo[2])));
					tempBuffer.setTextureRect(sf::IntRect(0,0,Inkball::CELL_SIZE,Inkball::CELL_SIZE));
					base.draw(tempBuffer);
				}
				else
				throw std::runtime_error("Level::loadPreview - failed due to unknown format");

			}
			else if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::TILE)) {



				if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::TileType::HOLE)) {
					size_t leftP = findNthOccur(entity, '<', 3);
					size_t rightp = findNthOccur(entity, '>', 3);
					if (leftP <= 0 || rightp <= 0 || pos[0] >= 15 || pos[1] >= 15)
						throw std::runtime_error("Level::loadPreview - failed due to unknown format");

					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setTexture(res.getTexture(std::tuple<int, int, int>(entityInfo[0], entityInfo[1], entityInfo[2])));
					base.draw(tempBuffer);


				}
				else if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::TileType::SPAWN)) {
					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setTexture(res.getTexture(std::tuple<int, int, int>(entityInfo[0], entityInfo[1], entityInfo[2])));
					base.draw(tempBuffer);
				}
					
				else if (entityInfo[1] == static_cast<std::size_t>(Inkball::Textures::TileType::TRACK)) {

					int left1 = findNthOccur(entity, '>', 2);
					auto vel = parseInp<float>(entity.substr(left1 + 2, findNthOccur(entity, '>', 3) - left1 + 1 ), 2);
					left1 = findNthOccur(entity,'>',3) + 2 ;
					auto dir = parseInp<std::size_t>(entity.substr(left1, 1), 1);
					if (left1 == -1)
						throw std::runtime_error("Level::loadPreview - failed due to unknown format");     
					tempBuffer.setPosition(pos[0] * Inkball::CELL_SIZE, pos[1] * Inkball::CELL_SIZE);
					tempBuffer.setOrigin(Inkball::CELL_SIZE / 2.0f, Inkball::CELL_SIZE / 2.0f);
					tempBuffer.rotate(dir[0] * -90.0f);
					tempBuffer.setPosition(pos[0] + Inkball::CELL_SIZE / 2.0f, pos[1] + Inkball::CELL_SIZE / 2.0f);
					base.draw(tempBuffer);
				}
				else
				throw std::runtime_error("Level::loadPreview - failed due to unknown format");


			}
						
			else if (entityInfo[0] != static_cast<std::size_t>(Inkball::Textures::EntityType::BALL)) {
				throw std::runtime_error("Level::loadPreview - unknown format ");
			}
		}
	}
	catch (...) {
		file.close();
		base.clear();
		base.draw(sf::Sprite(res.getTexture(std::tuple(static_cast<int>(Inkball::Textures::OtherType::ERROR_PREVIEW), 0, 0))));
	}
}

void Level::saveLevel(const std::string& filename)
{

}

void Level::clearLevel()
{
	m_balls.clear();
	m_levelHud.clear();
	for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		for (std::size_t j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			m_levelmap[i][j].clear();
}

void Level::updateLevel(sf::Time dt)
{
	for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		for (int j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			if (!m_levelmap[i][j].empty()) {
				Inkball::Action action;
				action.mtype = Inkball::ActionType::NONE;
				(m_levelmap[i][j].back())->update(dt,action);
				
				if (action.mtype == Inkball::ActionType::DELETE_SELF) 
					m_levelmap[i][j].pop_back();
			}
}




int Level::findNthOccur(const std::string &str, char ch, int N)
{
	int occur = 0;
	for (std::size_t i = 0; i < str.length(); i++) {
		if (str[i] == ch) 
			occur += 1;
		if (occur == N)
			return i;
	}
	return -1;
	
}

void Level::loadDisplay()
{
	if (m_balls.size() > 0) {
		std::sort(m_balls.begin(), m_balls.end(), [](Level::ballInfo& a, Level::ballInfo& b) {return  a.m_spawnAfter < b.m_spawnAfter; });
		int I = m_balls.size();
		for (auto i = m_balls.rbegin(); i != m_balls.rend(); i++) {
			if (i->m_supdate) {
				m_ballsDisplay.emplace_back(*(i->s_ball)->getSprite().getTexture());
				m_ballsDisplay.back().setScale(0.7, 0.7);
				m_ballsDisplay.back().setPosition(-15 + 23 * I--, 5);
			}
		}
	}
}

void Level::loadHud()
{
	
	Resource& res = Resource::getResourceHandle();
	m_levelHud.emplace_back(res.getTexture(std::tuple<int,int,int>(static_cast<int>(Inkball::Textures::OtherType::FLOOR),0,0)));

	//other textures
	m_levelHud[0].setPosition(0, Inkball::CELL_SIZE); // set floor;;
	m_levelHud.emplace_back(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::HUD), 0, 0)));                                   //level elements
	m_levelHud[1].setPosition(0, 0);

	//texts score, highscore, level timer

	for (int i=0; i<=3;i++)
	m_levelStr.emplace_back("0", res.getFont(Inkball::Fonts::ARCADE),15U);
 
	m_levelStr[0].setPosition(175.0f, 6.0f);
	m_levelStr[1].setPosition(325.0f, 6.0f);
	m_levelStr[2].setPosition(469.0f, 6.0f);
	m_levelStr[2].setFillColor(sf::Color::Black);
	m_levelStr[3].setPosition(510.0f, 6.0f);

	//.....
}

Block* Level::createBlock(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{   //id 0 , 1 , 2  corresponds to
	//   E   Ec  C   (entityType, entityClass,Color)
	const Resource& res = Resource::getResourceHandle();

	if (id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::COLORED))
		 return new Colored(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]));
	else if (id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::BROKEN))
		return new Broken(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]));
	else if (id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::DIRECTIONAL) || id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::DIRECTIONAL2)) {
		//direction 0 spriteDir 1 logicDir
		int left1 = findNthOccur(sub, '>', 2);
		if (left1 == -1)
			throw std::runtime_error("Level::loadLevel - failed due to unknown format");
		auto direction = parseInp<std::size_t>(sub.substr(left1 + 2, 3), 2);
		return new Directional(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), static_cast<Inkball::Direction>(direction[0]), static_cast<Inkball::Direction>(direction[1]), static_cast<Inkball::Textures::BlockType>(id[1]));
		
	}
	else if (id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::KEY))
		return new Key(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]));
		
	
	else if (id[1] == static_cast<std::size_t>(Inkball::Textures::BlockType::TIMER)) {
		//   <16,10><0,5,0><> timer format new last one amount of time to cycle b/n frames
		int left1 = findNthOccur(sub, '>', 2);
		auto tick = parseInp<float>(sub.substr(left1 + 2, 1), 1);
		return new Timer(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]),tick[0]);
	}
    throw std::runtime_error("Level::loadLevel - failed due to unknown format");
}

std::tuple<Ball*,float,bool> Level::createBall(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{   
	const Resource& res = Resource::getResourceHandle();
	size_t leftP  = findNthOccur(sub, '<', 3);
	size_t rightP = findNthOccur(sub, '>', 3);
	auto vel      = parseInp<float>(sub.substr(leftP + 1, rightP - (leftP + 1)),2);
	auto spawn    = parseInp<std::size_t>(sub.substr(rightP + 2,1),1);

	if (spawn[0] == 0)
		return { new Ball(sf::Vector2f(vel[0], vel[1]), sf::Vector2u(loc[0], loc[1]), static_cast<Inkball::Textures::Color>(id[2]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2]))) , 0.0f , false};
	else{
		leftP  = findNthOccur(sub,'<', 5);
		rightP = findNthOccur(sub,'>', 5);
	auto spawnTime= parseInp<float>(sub.substr(leftP + 1, rightP - (leftP + 1)), 1);
	return  { new Ball(sf::Vector2f(vel[0],vel[1]),sf::Vector2u(loc[0],loc[1]), static_cast<Inkball::Textures::Color>(id[2]),res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2]))) ,spawnTime[0],true};
	}
}

Tile* Level::createTile(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{
	//id 0 , 1 , 2, 3  corresponds to
	//   E   Ec  C  O  (entityType, entityClass,Color,Orientaion)
#if 0
	    < 0, 0 ><E, EC, C> track
		<0, 0><E, EC, C> spawn
		<0, 0><E, EC, C><NoKey><key1x, key1y, .............>associated keys g holees
		       6          13
			3   1,2,2,2,2,2
			4   1,2,3,4,5,6,7,8
		if key count  == 0 no need to parse further
#endif // 0

	    const Resource& res = Resource::getResourceHandle();
	
		if (id[1] == static_cast<std::size_t>(Inkball::Textures::TileType::HOLE)) {
			size_t leftP = findNthOccur(sub, '<', 3);
			size_t rightp = findNthOccur(sub, '>', 3);
			if (leftP <= 0 || rightp <= 0 || loc[0] >=15 || loc[1] >= 15)
				throw std::runtime_error("Level::loadLevel - failed due to unknown format");
			std::size_t keyCount = parseInp<std::size_t>(sub.substr(leftP + 1, rightp - (leftP + 1)), 1)[0];
			if (keyCount > 0) {
				leftP = findNthOccur(sub, '<', 4);
				auto keys = parseInp<std::size_t>(sub.substr(leftP + 1, (keyCount * 4) - 1), keyCount * 2);
				std::shared_ptr<std::vector<std::tuple<std::size_t, std::size_t>>>  sharedKeys = std::make_shared<std::vector<std::tuple<std::size_t, std::size_t>>>();
				if(keys.size()%2 != 0 )
					throw std::runtime_error("Level::loadLevel - failed due to unknown format");
				for (std::size_t i = 0; i < keys.size(); i += 2) 
					sharedKeys->emplace_back(keys[i], keys[i + 1]);

				
				m_levelmap[loc[0] + 1][loc[1]].emplace_back(new Hole(sf::Vector2u(loc[0] + 1, loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]),Hole::HolePiece(1), sharedKeys));
				m_levelmap[loc[0] + 1][loc[1] + 1].emplace_back(new Hole(sf::Vector2u(loc[0] + 1, loc[1] + 1), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(2), sharedKeys));
				m_levelmap[loc[0]][loc[1] + 1].emplace_back(new Hole(sf::Vector2u(loc[0], loc[1]  + 1), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(3), sharedKeys));
				return new Hole(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(0),sharedKeys);

			}
			m_levelmap[loc[0] + 1][loc[1]].emplace_back(new Hole(sf::Vector2u(loc[0] + 1, loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(1)));
			m_levelmap[loc[0] + 1][loc[1] + 1].emplace_back(new Hole(sf::Vector2u(loc[0] + 1, loc[1] + 1), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(2)));
			m_levelmap[loc[0]][loc[1] + 1].emplace_back(new Hole(sf::Vector2u(loc[0], loc[1] + 1), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(3)));
			return new Hole(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]), Hole::HolePiece(0));
        }
		else if (id[1] == static_cast<std::size_t>(Inkball::Textures::TileType::SPAWN))
			return new spawn(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])));
		else if (id[1] == static_cast<std::size_t>(Inkball::Textures::TileType::TRACK)) {

			int left1 = findNthOccur(sub, '>', 2);
			auto vel = parseInp<float>(sub.substr(left1 + 2, findNthOccur(sub, '>', 3) - left1 + 1 ), 2);
			if (left1 == -1)
				throw std::runtime_error("Level::loadLevel - failed due to unknown format");                                                                    //this velocity is used for track component  not for the track itself
			return new Track(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])),sf::Vector2f(vel[0],vel[1])); // set the velocity for track
		}
		else
		throw std::runtime_error("Level::loadLevel - failed due to unknown format");
}

void Level::ballInfo::status(sf::Time dt)
{
	if (m_spawnAfter > sf::Time::Zero)
		m_spawnAfter -= dt;
	else 
		m_spawnAfter = sf::Time::Zero;
}
