
#include <optional>
#include <SFML/Graphics.hpp>
#include "Indicator.h"
#include "GuiCardCollection.h"
#include "Match.h"
#include "Game.h"
#include "Button.h"
#include "GuiText.h"

#ifndef GUIMATCH_H_
#define GUIMATCH_H_

class GuiMatch final : public sf::Drawable {
public:
	GuiMatch(const Game* game);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	// update - get info about the match from match object and update Gui correspondingly
	void update(const Match& match);

	// Control elements clicked getters
	bool clickedHandCard(const sf::Event& ev) const;
	std::optional<size_t> clickedHandCardIndex(const sf::Event& ev) const;
	bool clickedFlipButton(const sf::Event& ev) const;
	std::optional<size_t> clickedFlipButtonIndex(const sf::Event& ev) const;
	bool clickedEndTurnButton(const sf::Event& ev) const;
	bool clickedStandButton(const sf::Event& ev) const;
	bool clickedForfeitButton(const sf::Event& ev) const;

private:
	void setupButtons(const Game* game);
	void setupBulbs(const Game* game);
	void setupText(const Game* game);
	void setupCards(const Game* game);

	void updateTurnBulbs(const Match& match);
	void updateSetBulbs(const Match& match);
	void updateScoreText(const Match& match);
	void updateFlipButtons(const Match& match);
	void updateHands(const Match& match);
	void updateTables(const Match& match);

	const Game* game_;

	inline static sf::Vector2f Size = GameConst::ScreenSize;

	// Bulbs
	BulbIndicator player_turn_bulb_;
	BulbIndicator AI_turn_bulb_;
	std::vector<BulbIndicator> player_sets_;
	std::vector<BulbIndicator> AI_sets_;

	inline static const std::string TurnBulbStyle = GameConst::TurnBulbStyle;
	inline static const std::string SetBulbStyle = GameConst::SetBulbStyle;
	inline static const size_t BulbPointCount = 50;

	inline static float PlayerTurnBulbRadius = 30;
	inline static const sf::Vector2f PlayerTurnBulbStart = { 40, 20 }; // Lef top corner of the bulb

	inline static const float SetPlayerBulbRadius = PlayerTurnBulbRadius * 0.5f;
	inline static const float SetPlayerBulbPadding = SetPlayerBulbRadius;
	inline static const sf::Vector2f PlayerSetBulbStart = { PlayerTurnBulbStart.x + PlayerTurnBulbRadius - SetPlayerBulbRadius, 
		PlayerTurnBulbStart.y + 2 * PlayerTurnBulbRadius + SetPlayerBulbPadding }; // Set the set bulbs to be smaller and bellow the turn bulb

	inline static const float AITurnBulbRadius = PlayerTurnBulbRadius;
	inline static const sf::Vector2f AITurnBulbStart = { Size.x - PlayerTurnBulbStart.x - 2 * AITurnBulbRadius, 
		PlayerTurnBulbStart.y }; // Symmetrical in relation to the player turn bulb

	inline static const float SetAIBulbRadius = SetPlayerBulbRadius;
	inline static const float SetAIBulbPadding = SetPlayerBulbPadding;
	inline static const sf::Vector2f AISetBulbStart = { Size.x - PlayerSetBulbStart.x - 2 * SetAIBulbRadius, PlayerSetBulbStart.y };


	// Cards
	GuiCardCollection player_hand_;
	GuiCardCollection player_table_;
	GuiCardCollection AI_hand_;
	GuiCardCollection AI_table_;

	inline static const unsigned int TableCardsPerLine = 3;
	inline static const unsigned int TableCardsTotal = GameConst::MaxCardsOnTable;
	inline static const float CardPadding = 10;

	inline static const float TableWidth = TableCardsPerLine * (GameConst::CardDimensions.x + CardPadding) + CardPadding;
	inline static const float HandWidth = GameConst::MaxCardsInHand * (GameConst::CardDimensions.x + CardPadding) + CardPadding;

	inline static const sf::Vector2f PlayerHandStart = { 150, 450 };
	inline static const sf::Vector2f PlayerTableStart = { PlayerHandStart.x + 0.5f * (GameConst::MaxCardsInHand - TableCardsPerLine) * 
		(GameConst::CardDimensions.x + CardPadding), 100 }; // Center table in relation to Hand
	inline static const sf::Vector2f AIHandStart = { Size.x - PlayerHandStart.x - HandWidth, PlayerHandStart.y }; // Symmetrical in relation with player hand
	inline static const sf::Vector2f AITableStart = { AIHandStart.x + 0.5f * (GameConst::MaxCardsInHand - TableCardsPerLine) * 
		(GameConst::CardDimensions.x + CardPadding), 100 };

	inline static const float TablesCenter = 0.5f * (PlayerTableStart.x + TableWidth + AITableStart.x); // Center between the two tables

	// Buttons
	Button end_turn_but_;
	Button stand_but_;
	Button forfeit_but_;
	std::vector<Button> flip_buttons_;

	inline static const float ButtonPadding = 10;
	inline static const sf::Vector2f ButtonSize = { 200, 70 };
	inline static const sf::Vector2f ButtonStart = { AIHandStart.x + 0.5f * (GameConst::MaxCardsInHand * 
		(GameConst::CardDimensions.x + CardPadding) + CardPadding - 3.f * (ButtonSize.x + ButtonPadding)), 
		AIHandStart.y + GameConst::CardDimensions.y + 3 * CardPadding};

	inline static const float FlipButtonPadding = CardPadding;
	inline static const sf::Vector2f FlipButtonSize = { GameConst::CardDimensions.x, 50 };
	inline static const sf::Vector2f FlipButtonStart = { PlayerHandStart.x, PlayerHandStart.y +
		GameConst::CardDimensions.y + 3 * CardPadding };

	// Text
	GuiText player_score_;
	GuiText AI_score_;

	GuiText player_name_;
	GuiText AI_name_;

	inline static const unsigned int TextCharSize = 50;
	inline static const std::string TextStyle = GameConst::TextStyle;

	inline static const sf::Vector2f PlayerScoreStart = { TablesCenter - 2 * TextCharSize, PlayerTableStart.y};
	inline static const sf::Vector2f AIScoreStart = { TablesCenter + TextCharSize, AITableStart.y };

	inline static const sf::Vector2f PlayerNameStart = { PlayerTableStart.x, PlayerTableStart.y - TextCharSize - 20 };
	inline static const sf::Vector2f AINameStart = { AITableStart.x, AITableStart.y - TextCharSize - 20 };


};

#endif // !GUIMATCH_H_
