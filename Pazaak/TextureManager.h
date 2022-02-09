
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

/* Texture Manager
* Handles textures so that they need only be loaded once
* Part of the Game class
*/
class TextureManager final {
public:
	// throws error on unsuccsessful load
	void loadTexture(const std::string& texture_name, const std::string& filename);
	// throws std::runtime_error on texture_name not found
	const sf::Texture& getTexture(const std::string& texture_name) const;
private:
	std::unordered_map<std::string, sf::Texture> textures_;
};

#endif // !TEXTUREMANAGER_H_
