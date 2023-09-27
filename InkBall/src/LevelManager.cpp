#include <filesystem>
#include <sys/stat.h>
#include "LevelManager.h"
#include "GameState.h"
#include "Resource.h"
#include "Util.h"

LevelManager::LevelManager(StateStack& stack, sf::RenderWindow* window):State(stack, window), m_pressed(false), m_game()
{
	const Resource& handle = Resource::getResourceHandle();
	m_backg.setTexture(handle.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::S_FLOOR), 0, 0)));
	m_arrowLeft.setTexture(handle.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_LEFT), 0, 0)));
	m_arrowRight.setTexture(handle.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::MENU_RIGHT), 0, 0)));

	m_arrowLeft.scale(0.8f,0.8f);
	m_arrowRight.scale(0.8f,0.8f);
	
	Util::centerOrigin(m_arrowLeft);
	Util::centerOrigin(m_arrowRight);

	m_arrowLeft.setPosition(Inkball::SCREEN_WIDTH / 8.5f, Inkball::SCREEN_HEIGHT / 2.0f);
	m_arrowRight.setPosition(Inkball::SCREEN_WIDTH / 1.1f, Inkball::SCREEN_HEIGHT / 2.0f);


	m_backg.setPosition(0, 0);

	m_game = [&stack, window, this]() { return new GameState(stack, window, std::move(this->m_levelFile) , this->getLevelPointer() ); };
	
	m_hover.setBuffer(handle.getSoundBuffer(Inkball::Sound::SoundEffects::HOVER));
	m_click.setBuffer(handle.getSoundBuffer(Inkball::Sound::SoundEffects::MENU_CLICK));

	m_level.setFont(handle.getFont(Inkball::Fonts::ARCADE));
	m_level.setCharacterSize(35U);
	m_level.setString("Level Select");
	m_level.setPosition(Inkball::SCREEN_WIDTH / 3.3f, Inkball::SCREEN_HEIGHT / 11.0f);
	m_level.setOutlineThickness(2);

	m_levelName = m_level;
	m_level.setString("TEST");
	m_level.setCharacterSize(30U);
	m_level.setPosition(Inkball::SCREEN_WIDTH / 2.f, Inkball::SCREEN_HEIGHT / 1.2f);
	
	relodLevel();

	Level::loadPreview(m_preview,m_levelFile[0].c_str()); 

	
	m_preview.display();
	m_prev.setTexture(m_preview.getTexture());
	m_prev.setScale(0.6f, 0.6f);
	m_prev.setPosition(118, 115);
}

bool LevelManager::draw()
{
	m_level.setString(m_levelFile[m_levelPointer]);
	Util::centerOrigin(m_level);
	m_window->draw(m_backg);
	m_window->draw(m_levelName);
	m_window->draw(m_level);
	m_window->draw(m_arrowLeft);
	m_window->draw(m_arrowRight);
	m_window->draw(m_prev);
	return false;
}   


bool LevelManager::update(sf::Time dt)
{
	if (m_prev.getGlobalBounds().contains(m_pos)) {

		m_prev.setColor(sf::Color::Yellow);
		if (m_pressed) {
			requestStackPop();
			requestCustomStackPush(m_game());
		}
		return false;
	}
	else
		m_prev.setColor(sf::Color::White);

	


	if (m_arrowLeft.getGlobalBounds().contains(m_pos)) {

		m_arrowLeft.setColor(sf::Color::Yellow);
		
		
		if (m_pressed) {
			
			m_click.play();
			m_levelPointer = m_levelPointer - 1 == -1 ? m_levelFile.size() - 1 : m_levelPointer - 1;
			m_preview.clear();
			Level::loadPreview(m_preview,m_levelFile[m_levelPointer].c_str());
			m_preview.setSmooth(1);
		
			m_preview.display();

		

			m_pressed = false;
		}
		

	}
	else
		m_arrowLeft.setColor(sf::Color::White);

     if (m_arrowRight.getGlobalBounds().contains(m_pos)) {
	
		m_arrowRight.setColor(sf::Color::Yellow);
		
		

		
		if (m_pressed) {
		
			m_levelPointer = (m_levelPointer + 1) % m_levelFile.size();

			m_click.play();

			m_preview.clear();
			Level::loadPreview(m_preview, m_levelFile[m_levelPointer].c_str());
			m_preview.setSmooth(1);
			m_preview.display();
			
			m_pressed = false;
		}
	
	
	}
    else
		 m_arrowRight.setColor(sf::Color::White);

	return false;
}

bool LevelManager::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		m_pos.x = (float)event.mouseMove.x;
		m_pos.y = (float)event.mouseMove.y;
	}
	if (event.type == sf::Event::MouseButtonPressed)
		m_pressed = true;

	
	return false;
}
void LevelManager::relodLevel()
{
	m_levelFile.clear();

	struct stat sb;

	for (const auto& entry : std::filesystem::directory_iterator("res/levels")) {
		std::filesystem::path outfilename = entry.path();
		std::string path = outfilename.string();
		std::cout << path << std::endl;
		if (stat(path.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
			m_levelFile.push_back(path.substr(path.find_last_of('\\') + 1, std::string::npos));
	}
}

