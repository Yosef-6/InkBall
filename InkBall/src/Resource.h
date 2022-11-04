#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Identifiers.h"
#include "ResourceHolder.h"
class Resource
{
public:
	Resource(const Resource& other) = delete;
	void init();
	void shutDown();
	static Resource& getResourceHandle();
	void loadTextures();  // requires textures to be .png
	void loadSoundEffects();
	sf::Texture& getTexture(const std::tuple<int,int,int>& ID);
	const sf::Texture& getTexture(const std::tuple<int, int, int>& ID) const;
	sf::Font& getFont(Inkball::Fonts ID);
	const sf::Font& getFont(Inkball::Fonts ID) const;
	const char* EntityResource[3] = { "Block","Ball","Tile" };  // Resource names must match actual resources
	const char* BlockResource[6] = { "Color","Broken","Directional","Directional2","Key","Timer" };
	const char* TileResource[3] = { "Hole","Track","Spawn" };
	const char* Color[5] = { "White","Blue","Orange","Green","Yellow" };
private:
	bool isInit = false;
	Resource() {};
	static Resource m_resource;
	ResourceHolder<sf::Texture,std::tuple<int,int,int>> m_textureHolder;
	ResourceHolder<sf::SoundBuffer, Inkball::Sound::SoundEffects> m_soundEffecteHolder;
	ResourceHolder<sf::Font, Inkball::Fonts>m_fontHolder;
};
