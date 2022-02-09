#include "FontManager.h"

/* Loads the font from the given file and registers it under the given name
* throws std::runtime_error on unsuccesfull load 
*/
void FontManager::loadFont(const std::string& font_name, const std::string& filename)
{
	sf::Font font;
	if (!font.loadFromFile(filename)) {
		throw std::runtime_error{ "Unable to load font " + font_name + " from: " + filename };
	}

	fonts_.emplace(font_name, font);
}

/* Get the font registered under the font_name
* Throws error on font name not found 
*/
const sf::Font& FontManager::getFont(const std::string& font_name) const
{
	return fonts_.at(font_name);
}
