#include "Resource.h"
#include <sstream>

Resource Resource::m_resource;

bool Resource::init()
{
	try {
		loadTextures();
		loadSoundEffects();
		loadFonts();
	}
	catch (...) {
		isInit = false;
		return false;
	}
	isInit = true;
	return true;
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
	for (int i = 0; i < static_cast<int>(Inkball::Textures::BlockType::END); i++) {
		for (int j = 0; j < static_cast<int>(Inkball::Textures::Color::END); j++) {
			if (i == static_cast<int>(Inkball::Textures::BlockType::TIMER) && j != static_cast<int>(Inkball::Textures::Color::WHITE))
				break;
			if ((i == static_cast<int>(Inkball::Textures::BlockType::DIRECTIONAL2) || i == static_cast<int>(Inkball::Textures::BlockType::KEY)) && (j == static_cast<int>(Inkball::Textures::Color::WHITE)))
				continue;
			buffer << "res/textures/" << EntityResource[entity] << BlockResource[i] << Color[j] << ".png";
			m_textureHolder.load({ entity,i,j }, buffer.str());
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

			buffer << "res/textures/" << EntityResource[entity] << TileResource[i] << Color[j] << ".png";
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
		buffer << "res/textures/" << EntityResource[entity] << Color[j] << ".png";
		m_textureHolder.load({ entity,0,j }, buffer.str());
		buffer.clear();
		buffer.str(std::string());
	}

	//load other textures types 


	m_textureHolder.load(std::tuple<int,int,int>(static_cast<int>(Inkball::Textures::OtherType::LOGO),0,0),"res/textures/InkBallLogo.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::FLOOR), 0, 0), "res/textures/floor.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_FLOOR), 0, 0), "res/textures/menuFloor.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::S_FLOOR), 0, 0), "res/textures/titleFloor.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ERROR_PREVIEW), 0, 0), "res/textures/errorPreview.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_LEFT), 0, 0), "res/textures/arrowLeft.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_RIGHT), 0, 0), "res/textures/arrowRight.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::HUD), 0, 0), "res/textures/hud.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::EDITER_HUD), 0, 0), "res/textures/lvlEditor.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ITEM_SELECT), 0, 0), "res/textures/itemSelect.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ITEMS), 0, 0), "res/textures/items.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::PAUSE), 0, 0), "res/textures/pause.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::CORRECT), 0, 0), "res/textures/correct.png");
	m_textureHolder.load(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ERROR), 0, 0), "res/textures/error.png");

}

void Resource::loadSoundEffects()
{
	m_soundEffecteHolder.load(Inkball::Sound::SoundEffects::HOVER, "res/soundEffects/hover.wav");
	m_soundEffecteHolder.load(Inkball::Sound::SoundEffects::MENU_CLICK, "res/soundEffects/menu_click.wav");
	m_soundEffecteHolder.load(Inkball::Sound::SoundEffects::LEVEL_SELECT, "res/soundEffects/level_click.wav");
	
}

void Resource::loadFonts()
{
	// load fonts
	m_fontHolder.load(Inkball::Fonts::TEXT1, "res/fonts/code/CODE Bold.otf");
	m_fontHolder.load(Inkball::Fonts::ARCADE, "res/fonts/arcade/ARCADECLASSIC.ttf");
}



const sf::Texture& Resource::getTexture(const std::tuple<int, int, int>& ID) const
{
	assert(isInit);
	return m_textureHolder.get(ID);
}



const sf::SoundBuffer& Resource::getSoundBuffer(Inkball::Sound::SoundEffects ID) const
{    
	assert(isInit);
	return m_soundEffecteHolder.get(ID);

}

const sf::Font& Resource::getFont(Inkball::Fonts ID) const
{
	assert(isInit);
	return m_fontHolder.get(ID);
}
