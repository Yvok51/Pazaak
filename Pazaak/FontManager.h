
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_

// Manages sf::Fonts in memory
class FontManager final {
public:
	// throws std::runtime_error on unsuccesfull load
	void loadFont(const std::string& font_name, const std::string& filename);
	// throws error on font name not found
	const sf::Font& getFont(const std::string& font_name) const;
private:
	std::unordered_map<std::string, sf::Font> fonts_;
};

#endif // !FONTMANAGER_H_

