
#include <string>
#include <SFML/Graphics.hpp>

#ifndef GAMECONST_H_
#define GAMECONST_H_

// Struct containing constants used throughout the program
// or ones tied to the resources
struct GameConst {
	static inline const sf::Vector2f ScreenSize = { 1280.f, 720.f };
	static inline const sf::Vector2f PosMul = { ScreenSize.x / 1280.f, ScreenSize.y / 720.f };

	static inline const int TotalSets = 3;
	static inline const int MaxCardsInHand = 4;
	static inline const int MaxCardsOnTable = 9;
	static inline const int ScoreLimit = 20;
	static inline const int DeckSelectLimit = 10; // How many cards is the player allowed to select during deck build
	static inline const int CampaignNumberOfEnemies = 4;
	static inline const int TotalNumberOfCards = 22;
	static inline const int CampaignStartCredits = 150;

	static inline const std::string PlayerName = "Player";
	static inline const std::string OpponentName = "Opponent";

	static inline const std::string TextureBaseDir = "Textures";
	static inline const std::string FontBaseDir = "Fonts";
	static inline const std::string SaveLocation = "Saves/save.xml";

	static inline const std::string ButtonStyle = "Button";
	static inline const std::string CardStyle = "Card";
	static inline const std::string TransparentRectStyle = "TransparentRect";
	static inline const std::string TextStyle = "Text";
	static inline const std::string TurnBulbStyle = "TurnBulb";
	static inline const std::string SetBulbStyle = "SetBulb";
	static inline const std::string PromptStyle = "Prompt";

	static inline const sf::Vector2f CardDimensions = sf::Vector2f(80.f, 100.f);
	static inline const unsigned int CardCharSize = 20; // determined by the texture -> in GameConst
};


#endif // !GAMECONST_H_
