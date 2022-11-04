#include "Resource.h"
#include <sstream>

Resource Resource::m_resource;

void Resource::init()
{
	loadTextures();
	//loadSoundEffects();
	isInit = true;
}

void Resource::shutDown()
{
	m_textureHolder.deleteResource();
	m_fontHolder.deleteResource();
	m_soundEffecteHolder.deleteResource();
}

Resource& Resource::getResourceHandle()
{ 
	return m_resource;
}

void Resource::loadTextures() //loads all the textures
{
	// load all block textures
	std::stringstream buffer;
	int entity = static_cast<int>(Inkball::Textures::EntityType::BLOCK);
	// load blocks
	for (int i = 0; i < static_cast<int>(Inkball::Textures::BlockType::END);i++) {
		for (int j = 0; j < static_cast<int>(Inkball::Textures::Color::END);j++) {
			if (i == static_cast<int>(Inkball::Textures::BlockType::TIMER) && j != static_cast<int>(Inkball::Textures::Color::WHITE))
				break;
			if ((i == static_cast<int>(Inkball::Textures::BlockType::DIRECTIONAL2) || i == static_cast<int>(Inkball::Textures::BlockType::KEY)) && (j == static_cast<int>(Inkball::Textures::Color::WHITE)))
				continue;
			buffer <<"res/textures/" << EntityResource[entity] << BlockResource[i] << Color[j] << ".png";
			m_textureHolder.load({entity,i,j},buffer.str());
			buffer.clear();
			buffer.str(std::string());
		}
	}
	
	// load Entity type tile 
	// if block is set i dont have track and spawn with diffrent colors i coud add them as a feature
	buffer.clear();
	buffer.str(std::string());
	entity = static_cast<int>(Inkball::Textures::EntityType::TILE);
	for (int i = 0; i < static_cast<int>(Inkball::Textures::TileType::END); i++) {
		for (int j = 0; j < static_cast<int>(Inkball::Textures::Color::END); j++) {
			if ((i == static_cast<int>(Inkball::Textures::TileType::TRACK) || i == static_cast<int>(Inkball::Textures::TileType::SPAWN)) && j != static_cast<int>(Inkball::Textures::Color::WHITE))
				break;
				
			    buffer <<"res/textures/" << EntityResource[entity] << TileResource[i] << Color[j] << ".png";
				m_textureHolder.load({ entity,i,j }, buffer.str());
				buffer.clear();
				buffer.str(std::string());
		}
	}
	buffer.clear();
	buffer.str(std::string());
	entity = static_cast<int>(Inkball::Textures::EntityType::BALL);
	// i = 0 there is no subclass of a ball entity like blocks.. so i is set 0
	for (int j = 0; j < static_cast<int>(Inkball::Textures::Color::END); j++) {
		buffer<<"res/textures/" << EntityResource[entity]  << Color[j] << ".png";
		m_textureHolder.load({ entity,0,j }, buffer.str());
		buffer.clear();
		buffer.str(std::string());
	}

	//load other textures types 
	m_textureHolder.load(std::tuple<int,int,int>(static_cast<int>(Inkball::Textures::OtherType::LOGO),0,0),"res/textures/InkBallLogo.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::FLOOR), 0, 0), "res/textures/floor.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::S_FLOOR), 0, 0), "res/textures/splashFloor.png");
	// load fonts
	m_fontHolder.load(Inkball::Fonts::TITLE,"res/fonts/code/CODE Bold.otf");
	m_fontHolder.load(Inkball::Fonts::TEXT1, "res/fonts/bellerose/Bellerose.ttf");
	m_fontHolder.load(Inkball::Fonts::TEXT2, "res/fonts/lemon_milk/LEMONMILK-Light.otf");
	m_fontHolder.load(Inkball::Fonts::TEXT3, "res/fonts/nasalization/nasalization-rg.otf");

}

void Resource::loadSoundEffects()
{
	//m_soundEffecteHolder.load();
}

sf::Texture& Resource::getTexture(const std::tuple<int, int, int>& ID)
{   
	assert(isInit);
	return m_textureHolder.get(ID);
}

const sf::Texture& Resource::getTexture(const std::tuple<int, int, int>& ID) const
{
	assert(isInit);
	return m_textureHolder.get(ID);
}

sf::Font& Resource::getFont(Inkball::Fonts ID)
{
	assert(isInit);
	return m_fontHolder.get(ID);
}

const sf::Font& Resource::getFont(Inkball::Fonts ID) const
{
	assert(isInit);
	return m_fontHolder.get(ID);
}
