
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

/* Load a texture from the given file and registers it under the texture_name
* Throws error on unsuccsessful load 
*/
void TextureManager::loadTexture(const std::string& texture_name, const std::string& filename)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filename)) {
		throw std::runtime_error{ "Unable to load texture " + texture_name + " from: " + filename };
	}
	textures_.emplace(texture_name, texture);
}

/* Get the texture registered under the given texture_name
* Throws std::runtime_error if texture_name is not a loaded texture
*/
const sf::Texture& TextureManager::getTexture(const std::string& texture_name) const
{
	return textures_.at(texture_name);
}
