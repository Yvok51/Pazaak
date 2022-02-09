
#include <string>
#include <optional>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Deck.h"
#include "Game.h"
#include "Indicator.h"
#include "GuiCardCollection.h"

#ifndef MATCH_H_
#define MATCH_H_

enum class SetResult {
	Win,
	Loss,
	Draw
};

class Match final {
public:
	Match(const Game* game, MatchObserver* parent, const std::string& player_name, const std::string& opponent_name, Deck&& player_deck, Deck&& AI_deck);

	// Modifiers

	/* Flip the value of the player's Hand card, if Card is not flipable does nothing,
	if index is out of range throws std::out_of_range */
	void playerFlipHandCard(size_t index);
	/* Plays the Player's Hand Card onto the table, if player cannot play given Card, does nothing,
	if index is out of range throws std::out_of_range */
	void playerUseCard(size_t index);
	void playerEndTurn();
	void playerStand();

	/* Flip the value of the AI's Hand card, if Card is not flipable does nothing,
	if index is out of range throws std::out_of_range */
	void AIFlipHandCard(size_t index);
	/* Plays the AI's Hand Card onto the table, if AI cannot play given Card, does nothing,
	if index is out of range throws std::out_of_range */
	void AIUseCard(size_t index);
	void AIEndTurn();
	void AIStand();

	void newSet();

	void signalMatchEnd();

	void clearGuiUpdateNeeded() { update_needed_ = false; }

	// Getters
	bool isGuiUpdateNeeded() const { return update_needed_; }

	/* Returns std::optional with appropriate Card observer pointer, if index is in range */
	std::optional<const Card*> getPlayerHandCard(size_t index) const { return human_player_.getHandCard(index); };
	std::optional<const Card*> getAIHandCard(size_t index) const { return AI_player_.getHandCard(index); };
	std::optional<const Card*> getPlayerTableCard(size_t index) const { return human_player_.getTableCard(index); };
	std::optional<const Card*> getAITableCard(size_t index) const { return AI_player_.getTableCard(index); };

	int getPlayerScore() const { return human_player_.getScore(); }
	int getAIScore() const { return AI_player_.getScore(); }
	size_t getPlayerWonSets() const { return human_player_.getSetsWon(); }
	size_t getAIWonSets() const { return AI_player_.getSetsWon(); }

	bool isPlayersTurn() const { return player_turn_; }
	bool isMatchOngoing() const { return game_ongoing_; }
	bool playerPlayedTieBraker() const { return human_player_.playedTiebraker(); }
	bool isPlayerStanding() const { return human_player_.stands(); }
	bool isAIStanding() const { return AI_player_.stands(); }
	bool isPlayerHandCardFlipable(size_t index) const { return human_player_.isHandCardFlipable(index); }
	bool isAIHandCardFlipable(size_t index) const { return AI_player_.isHandCardFlipable(index); }
	bool playerWon() const { return !isMatchOngoing() && human_player_.getSetsWon() == GameConst::TotalSets; }

	const std::string& getPlayerName() const { return human_player_.getName(); }
	const std::string& getAIName() const { return AI_player_.getName(); }

private:
	void endSet();
	void createNewDeck();
	void setGuiUpdateNeeded() { update_needed_ = true; };
	void flipWhoStarts() { player_started_set_ = !player_started_set_; }
	bool doesPlayerStart() { return player_started_set_; }
	void changeTurn();
	void newTurn();
	
	bool game_ongoing_;
	bool update_needed_;
	bool player_turn_;
	bool player_started_set_;

	Deck main_deck_;

	Player human_player_;
	Player AI_player_;

	MatchObserver* parent_;

	inline static const int MaxSets = GameConst::TotalSets;
	inline static const int ScoreLimit = GameConst::ScoreLimit;
	inline static const int CardLimit = GameConst::MaxCardsOnTable;
};

#endif // !MATCH_H
