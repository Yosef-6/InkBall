#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace Util {

	static void centerOrigin(sf::Sprite& sprite)
	{
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	static void centerOrigin(sf::Text& text)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}


	

}