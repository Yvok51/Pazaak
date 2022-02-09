
#include <string>
#include <SFML/Graphics.hpp>
#include "Indicator.h"
#include "GuiCardCollection.h"
#include "Button.h"
#include "GuiMatch.h"

GuiMatch::GuiMatch(const Game* game) : game_{ game },
	AI_hand_{ game->getStyle(GuiCardCollection::Style) }, player_hand_{ game->getStyle(GuiCardCollection::Style) }, 
	AI_table_{ game->getStyle(GuiCardCollection::Style) }, player_table_{ game->getStyle(GuiCardCollection::Style) },
	forfeit_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "Forfeit" },
	end_turn_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "End Turn" },
	stand_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "Stand" },
	player_score_{ game->getStyle(TextStyle), TextCharSize }, AI_score_{ game->getStyle(TextStyle), TextCharSize },
	player_name_{ game->getStyle(TextStyle), TextCharSize }, AI_name_{ game->getStyle(TextStyle), TextCharSize },
	player_sets_{ GameConst::TotalSets }, AI_sets_{ GameConst::TotalSets }, flip_buttons_{ GameConst::MaxCardsInHand }
{
	setupBulbs(game);
	setupButtons(game);
	setupText(game);
	setupCards(game);
}

void GuiMatch::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(forfeit_but_, states);
	target.draw(end_turn_but_, states);
	target.draw(stand_but_, states);
	target.draw(player_turn_bulb_, states);
	target.draw(AI_turn_bulb_, states);
	target.draw(player_hand_, states);
	target.draw(AI_hand_, states);
	target.draw(player_table_, states);
	target.draw(AI_table_, states);
	target.draw(player_score_, states);
	target.draw(AI_score_, states);
	target.draw(player_name_, states);
	target.draw(AI_name_, states);
	for (auto&& bulb : player_sets_) {
		target.draw(bulb, states);
	}
	for (auto&& bulb : AI_sets_) {
		target.draw(bulb, states);
	}
	for (auto&& flipButton : flip_buttons_) {
		target.draw(flipButton, states);
	}
}

void GuiMatch::update(const Match& match)
{
	updateTurnBulbs(match);
	updateSetBulbs(match);
	updateScoreText(match);
	updateFlipButtons(match);
	updateHands(match);
	updateTables(match);
}

bool GuiMatch::clickedHandCard(const sf::Event& ev) const
{
	return player_hand_.clicked(ev);
}

std::optional<size_t> GuiMatch::clickedHandCardIndex(const sf::Event& ev) const
{
	return player_hand_.clickedIndex(ev);
}

bool GuiMatch::clickedFlipButton(const sf::Event& ev) const
{
	for (auto&& flip_button : flip_buttons_) {
		if (flip_button.clicked(ev)) {
			return true;
		}
	}
	return false;
}

std::optional<size_t> GuiMatch::clickedFlipButtonIndex(const sf::Event& ev) const
{
	for (size_t i = 0; i < flip_buttons_.size(); i++)
	{
		if (flip_buttons_[i].clicked(ev)) {
			return i;
		}
	}
	return std::nullopt;
}

bool GuiMatch::clickedEndTurnButton(const sf::Event& ev) const
{
	return end_turn_but_.clicked(ev);
}

bool GuiMatch::clickedStandButton(const sf::Event& ev) const
{
	return stand_but_.clicked(ev);
}

bool GuiMatch::clickedForfeitButton(const sf::Event& ev) const
{
	return forfeit_but_.clicked(ev);
}

void GuiMatch::setupButtons(const Game* game)
{
	end_turn_but_.setPosition(ButtonStart.x, ButtonStart.y);
	stand_but_.setPosition(ButtonStart.x + ButtonSize.x + ButtonPadding, ButtonStart.y);
	forfeit_but_.setPosition(ButtonStart.x + 2 * (ButtonSize.x + ButtonPadding), ButtonStart.y);

	float i = 0;
	for (auto&& flip_button : flip_buttons_) {
		flip_button.setSize(FlipButtonSize.x, FlipButtonSize.y);
		flip_button.setText("Flip");
		flip_button.setStyle(game->getStyle(GameConst::ButtonStyle));
		flip_button.setPosition(FlipButtonStart.x + FlipButtonPadding + i * (FlipButtonSize.x + FlipButtonPadding), FlipButtonStart.y);
		i++;
	}
}

void GuiMatch::setupBulbs(const Game* game)
{
	player_turn_bulb_.setStyle(game->getStyle(TurnBulbStyle));
	player_turn_bulb_.setRadius(PlayerTurnBulbRadius);
	player_turn_bulb_.setPosition(PlayerTurnBulbStart.x, PlayerTurnBulbStart.y);
	player_turn_bulb_.setPointCount(BulbPointCount);

	AI_turn_bulb_.setStyle(game->getStyle(TurnBulbStyle));
	AI_turn_bulb_.setRadius(AITurnBulbRadius);
	AI_turn_bulb_.setPosition(AITurnBulbStart.x, AITurnBulbStart.y);
	AI_turn_bulb_.setPointCount(BulbPointCount);


	for (size_t i = 0; i < player_sets_.size(); i++)
	{
		player_sets_[i].setStyle(game->getStyle(SetBulbStyle));
		player_sets_[i].setRadius(SetPlayerBulbRadius);
		player_sets_[i].setPosition(PlayerSetBulbStart.x, PlayerSetBulbStart.y + i * (2 * SetPlayerBulbRadius + SetPlayerBulbPadding));
		player_sets_[i].setPointCount(BulbPointCount);

	}
	for (size_t i = 0; i < player_sets_.size(); i++)
	{
		AI_sets_[i].setStyle(game->getStyle(SetBulbStyle));
		AI_sets_[i].setRadius(SetAIBulbRadius);
		AI_sets_[i].setPosition(AISetBulbStart.x, AISetBulbStart.y + i * (2 * SetAIBulbRadius + SetAIBulbPadding));
		AI_sets_[i].setPointCount(BulbPointCount);

	}
}

void GuiMatch::setupText(const Game* game)
{
	player_score_.setPosition(PlayerScoreStart);
	AI_score_.setPosition(AIScoreStart);
	player_name_.setPosition(PlayerNameStart);
	AI_name_.setPosition(AINameStart);

	player_score_.setText("0");
	AI_score_.setText("0");
	player_name_.setText("Player");
	AI_name_.setText("Opponent");
}

void GuiMatch::setupCards(const Game* game)
{
	player_table_.setPosition(PlayerTableStart);
	player_table_.setCardDimensions(GameConst::CardDimensions);
	player_table_.setSize(player_table_.getWidthToFitCards(TableCardsPerLine), player_table_.getHeightToFitCards(TableCardsTotal / TableCardsPerLine));
	player_table_.setSpacing(CardPadding);

	AI_table_.setPosition(AITableStart);
	AI_table_.setCardDimensions(GameConst::CardDimensions);
	AI_table_.setSize(AI_table_.getWidthToFitCards(TableCardsPerLine), AI_table_.getHeightToFitCards(TableCardsTotal / TableCardsPerLine));
	AI_table_.setSpacing(CardPadding);

	player_hand_.setPosition(PlayerHandStart);
	player_hand_.setCardDimensions(GameConst::CardDimensions);
	player_hand_.setSize(player_hand_.getWidthToFitCards(GameConst::MaxCardsInHand), player_hand_.getHeightToFitCards(1));
	player_hand_.setSpacing(CardPadding);

	AI_hand_.setPosition(AIHandStart);
	AI_hand_.setCardDimensions(GameConst::CardDimensions);
	AI_hand_.setSize(AI_hand_.getWidthToFitCards(GameConst::MaxCardsInHand), AI_hand_.getHeightToFitCards(1));
	AI_hand_.setSpacing(CardPadding);
}

void GuiMatch::updateTurnBulbs(const Match& match)
{
	if (match.isPlayersTurn()) {
		player_turn_bulb_.lightup();
		AI_turn_bulb_.dim();
	}
	else {
		player_turn_bulb_.dim();
		AI_turn_bulb_.lightup();
	}
}

void GuiMatch::updateSetBulbs(const Match& match)
{
	for (size_t i = 0; i < player_sets_.size(); i++)
	{
		if (i < match.getPlayerWonSets()) {
			player_sets_[i].lightup();
		}
		else {
			player_sets_[i].dim();
		}
	}
	for (size_t i = 0; i < AI_sets_.size(); i++)
	{
		if (i < match.getAIWonSets()) {
			AI_sets_[i].lightup();
		}
		else {
			AI_sets_[i].dim();
		}
	}
}

void GuiMatch::updateScoreText(const Match& match)
{
	player_score_.setText(std::to_string(match.getPlayerScore()));
	AI_score_.setText(std::to_string(match.getAIScore()));
	player_name_.setText(match.getPlayerName());
	AI_name_.setText(match.getAIName());
}

void GuiMatch::updateFlipButtons(const Match& match)
{
	for (size_t i = 0; i < flip_buttons_.size(); i++)
	{
		flip_buttons_[i].setClickable(match.isPlayerHandCardFlipable(i));
	}
}

void GuiMatch::updateHands(const Match& match)
{
	// Player Hand
	for (size_t i = 0; i < GameConst::MaxCardsInHand; i++)
	{
		if (auto card = match.getPlayerHandCard(i)) {
			if (i < player_hand_.size()) { // GUI Card is defined
				player_hand_[i].update(game_, card.value());
			}
			else { // Match player hand has more cards than we
				player_hand_.add(GuiCard{ card.value(), game_, true });
			}
		}
		else { // No more cards defined in match hand -> delete the remaining GUI cards
			if (i < player_hand_.size()) {
				player_hand_.remove(i);
			}
		}
	}
	for (size_t i = 0; i < GameConst::MaxCardsInHand; i++)
	{
		if (auto card = match.getAIHandCard(i)) {
			if (i < AI_hand_.size()) { // GUI Card is defined
				AI_hand_[i].update(game_, card.value());
			}
			else { // Match player hand has more cards than we
				AI_hand_.add(GuiCard{ card.value(), game_, false, true });
			}
		}
		else { // No more cards defined in match hand -> delete the remaining GUI cards
			if (i < AI_hand_.size()) {
				AI_hand_.remove(i);
			}
		}
	}
}

void GuiMatch::updateTables(const Match& match)
{
	for (int i = 0; i < GameConst::MaxCardsOnTable; i++)
	{
		if (auto card = match.getPlayerTableCard(i)) {
			if (i < static_cast<int>(player_table_.size())) { // GUI Card is defined
				player_table_[i].update(game_, card.value());
			}
			else { // Match player table has more cards than we
				player_table_.add(GuiCard{ card.value(), game_ });
			}
		}
		else { // No more cards on the match table -> delete the remaining GUI cards
			if (i < static_cast<int>(player_table_.size())) {
				player_table_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}
	for (int i = 0; i < GameConst::MaxCardsOnTable; i++)
	{
		if (auto card = match.getAITableCard(i)) {
			if (i < static_cast<int>(AI_table_.size())) { // GUI Card is defined
				AI_table_[i].update(game_, card.value());
			}
			else { // Match player table has more cards than we
				AI_table_.add(GuiCard{ card.value(), game_ });
			}
		}
		else { // No more cards on the match table -> delete the remaining GUI cards
			if (i < static_cast<int>(AI_table_.size())) {
				AI_table_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}
}
