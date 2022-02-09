
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "GuiStyle.h"
#include "CardCollection.h"
#include "GameStateHandler.h"
#include "Campaign.h"

#ifndef GAME_H_
#define GAME_H_

class GameState;

/* Main class containing sf::RenderWindow, current game state, campaign and texture, font and styles managers */
class Game {
public:
	Game(unsigned int width, unsigned int height, unsigned int style_flags);

	void run();

	// throws error if searched for resource is not found
	const sf::Texture& getTexture(const std::string& texture_name) const { return tex_mgr_.getTexture(texture_name); }
	// throws error if searched for resource is not found
	const GuiStyle& getStyle(const std::string& style_name) const { return stylesheets_.at(style_name); }
	const sf::Sprite& getBackground() const { return background_; }
	CardCollection getFullCardCollection() const;
	sf::Vector2f getWindowCenter() const { return sf::Vector2f(window_.getSize()) * 0.5f; }

	Campaign& getCampaign() { return campaign_; }

	// Modify state stack
	void pushState(std::unique_ptr<GameState> state);
	void popState();
	void changeState(std::unique_ptr<GameState> state);
	std::unique_ptr<GameState>& peekState();

	void loadCampaign();
	void saveCampaign();

private:
	bool running() { return window_.isOpen(); }

	void load();
	void loadTextures();
	void loadFonts();
	void loadStyles();
	void setBackground();

	GameStateHandler state_handler;
	sf::RenderWindow window_;

	Campaign campaign_;

	sf::Sprite background_;
	TextureManager tex_mgr_;
	FontManager font_mgr_;
	std::unordered_map<std::string, GuiStyle> stylesheets_;
};

#endif // !GAME_H_
