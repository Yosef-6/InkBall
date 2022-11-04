#include "Level.h"
#include <fstream>
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


Level::Level():m_levelNo(1)
{
	loadLevel(m_levelNo);
}

Level::Level(unsigned short level):m_levelNo(level)
{
	loadLevel(m_levelNo);
}

void Level::loadLevel(unsigned short level) // exceptions are a handled in game state
{
	std::stringstream buffer;
	std::string entity;
	buffer <<"res/levels/Level" << level<<".lvl";
	std::ifstream file(buffer.str());
	if (!file.is_open()) 
		throw std::runtime_error("Level::loadLevel - Failed to load " + buffer.str());
	buffer.str(std::string());
	while (std::getline(file, entity)) {
		auto pos    = parseInp<std::size_t>(entity.substr(1,findNthOccur(entity,'>',1) - 1), 3);
		std::size_t cellx = pos[0];
		std::size_t celly = pos[1];
		if(cellx >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE || celly >= Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE)
			throw std::runtime_error("Level::loadLevel - unknown format ");
	//	std::cout<<cellx<<"   "<<celly<<std::endl;
	    //E entity Ec entity class C color O orirentation
		int left =  findNthOccur(entity, '<', 2) + 1; //6
		auto entityInfo = parseInp<std::size_t>(entity.substr(left, findNthOccur(entity, '>', 2) - left), 4);
		if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::BLOCK))
			m_levelmap[cellx][celly].emplace_back(createBlock(pos, entityInfo, entity));
		else if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::TILE))
			m_levelmap[cellx][celly].emplace_back(createTile(pos, entityInfo, entity));
		else if (entityInfo[0] == static_cast<std::size_t>(Inkball::Textures::EntityType::BALL)) {
			//ball is special object is not fixd inside the table (moves over the table)
			auto [ball, spawn] = createBall(pos, entityInfo, entity);
			m_balls.emplace_back( ball,spawn );
		}
		else
		throw std::runtime_error("Level::loadLevel - unknown format ");
	}
	file.close();
	m_levelNo = level;

	//load backg if it has not been loaded previously
	loadHud();
}

void Level::loadNext()
{
	
	clearLevel();
	loadLevel(m_levelNo++);
}

void Level::loadPrev()
{
	
	clearLevel();
	loadLevel(m_levelNo--);
}

void Level::saveLevel(const std::string& filename)
{

}

void Level::clearLevel()
{
	m_balls.clear();
	for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		for (std::size_t j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			m_levelmap[i][j].clear();
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

void Level::loadHud()
{
	
	Resource& res = Resource::getResourceHandle();
	m_levelHud.emplace_back(new sf::Sprite (res.getTexture(std::tuple<int,int,int>(static_cast<int>(Inkball::Textures::OtherType::FLOOR),0,0))));
	//other textures


	m_levelHud[0]->setPosition(0, Inkball::CELL_SIZE); // set floor
	//.....
}

Block* Level::createBlock(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{   //id 0 , 1 , 2, 3  corresponds to
	//   E   Ec  C  O  (entityType, entityClass,Color,Orientaion)
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
		
	
	else if (id[1]  == static_cast<std::size_t>(Inkball::Textures::BlockType::TIMER))
		return new Timer(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])), static_cast<Inkball::Textures::Color>(id[2]));
		
    throw std::runtime_error("Level::loadLevel - failed due to unknown format");
}

std::tuple<Ball*,float> Level::createBall(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{   
	const Resource& res = Resource::getResourceHandle();
	size_t leftP  = findNthOccur(sub, '<', 3);
	size_t rightP = findNthOccur(sub, '>', 3);
	auto vel      = parseInp<float>(sub.substr(leftP + 1, rightP - (leftP + 1)),2);
	auto spawn    = parseInp<std::size_t>(sub.substr(rightP + 2,1),1);

	if (spawn[0] <= 0)
		return { new Ball(sf::Vector2f(vel[0], vel[1]), sf::Vector2u(loc[0], loc[1]), static_cast<Inkball::Textures::Color>(id[2]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2]))) , 0.0f };
	else{
		leftP  = findNthOccur(sub,'<', 5);
		rightP = findNthOccur(sub,'>', 5);
	auto spawnTime= parseInp<float>(sub.substr(leftP + 1, rightP - (leftP + 1)), 1);
	return  { new Ball(sf::Vector2f(vel[0],vel[1]),sf::Vector2u(loc[0],loc[1]), static_cast<Inkball::Textures::Color>(id[2]),res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2]))) ,spawnTime[0]};
	}
}

Tile* Level::createTile(const std::vector<std::size_t>& loc,const std::vector<std::size_t>& id, const std::string& sub)
{
	//id 0 , 1 , 2, 3  corresponds to
	//   E   Ec  C  O  (entityType, entityClass,Color,Orientaion)
#if 0
	    < 0, 0 > <E, EC, C, o> track
		<0, 0><E, EC, C, o> spawn
		<0, 0><E, EC, C, o><NoKey><key1x, key1y, .............>associated keys g holees
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
		else if (id[1] == static_cast<std::size_t>(Inkball::Textures::TileType::TRACK))
			return new Track(sf::Vector2u(loc[0], loc[1]), res.getTexture(std::tuple<int, int, int>(id[0], id[1], id[2])));
		else
		throw std::runtime_error("Level::loadLevel - failed due to unknown format");
}

void Level::ballInfo::status(sf::Time dt)
{
	if (s_spawnAfter > sf::Time::Zero)
		s_spawnAfter -= dt;
	else if(s_ready!=true)
		s_ready = true;
	
}
