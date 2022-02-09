
#include <cassert>
#include <memory>
#include "AI.h"
#include "Card.h"
#include "Match.h"
#include "Game.h"
#include "MatchObserver.h"

Match::Match(const Game* game, MatchObserver* parent, const std::string& player_name, const std::string& opponent_name, Deck&& player_deck, Deck&& AI_deck) : 
	human_player_{ player_name,  std::move(player_deck) }, AI_player_{ opponent_name, std::move(AI_deck) }, parent_{parent}, game_ongoing_{ true }, 
	player_turn_{ true }, update_needed_{ true }, player_started_set_{ false }
{
	newSet();
}

void Match::createNewDeck()
{
	main_deck_.clear();

	main_deck_.add(std::make_unique<DealerCard>(1), 4);
	main_deck_.add(std::make_unique<DealerCard>(2), 4);
	main_deck_.add(std::make_unique<DealerCard>(3), 4);
	main_deck_.add(std::make_unique<DealerCard>(4), 4);
	main_deck_.add(std::make_unique<DealerCard>(5), 4);
	main_deck_.add(std::make_unique<DealerCard>(6), 4);
	main_deck_.add(std::make_unique<DealerCard>(7), 4);
	main_deck_.add(std::make_unique<DealerCard>(8), 4);
	main_deck_.add(std::make_unique<DealerCard>(9), 4);
	main_deck_.add(std::make_unique<DealerCard>(10), 4);

	main_deck_.shuffle();
}

void Match::changeTurn()
{
	player_turn_ = !player_turn_;
}

void Match::newTurn()
{
	if (player_turn_) {
		if (human_player_.canDrawCard()) {
			human_player_.drawCard(main_deck_.drawCard());
		}
		else { // Player cannot draw -> loses set
			endSet();
		}
	}
	else {
		if (AI_player_.canDrawCard()) {
			AI_player_.drawCard(main_deck_.drawCard());
		}
		else { // AI cannot draw -> loses set
			endSet();
		}
	}
}

void Match::playerFlipHandCard(size_t index)
{
	assert(isPlayersTurn());

	if (human_player_.isHandCardFlipable(index)) {
		human_player_.flipCard(index);
		setGuiUpdateNeeded();
	}
}

void Match::playerUseCard(size_t index)
{
	assert(isPlayersTurn());

	if (!human_player_.playedCard()) {
		human_player_.useCard(index);
		setGuiUpdateNeeded();
	}
}

void Match::playerEndTurn()
{
	assert(isPlayersTurn());

	human_player_.notifyTurnEnd();
	if (human_player_.getScore() > ScoreLimit) { // Player overdrew
		endSet();
	}
	else {
		if (!AI_player_.stands()) { // If AI stands repeat turn
			changeTurn();
		}
		newTurn();
	}
	setGuiUpdateNeeded();
}

void Match::playerStand()
{
	assert(isPlayersTurn());

	human_player_.notifyStand();
	if (human_player_.getScore() > ScoreLimit) {
		endSet();
	}
	else {
		if (!AI_player_.stands()) {
			changeTurn();
			newTurn();
		}
		else {
			endSet();
		}
	}
	setGuiUpdateNeeded();

}

void Match::AIFlipHandCard(size_t index)
{
	assert(!isPlayersTurn());

	if (AI_player_.isHandCardFlipable(index)) {
		AI_player_.flipCard(index);
		setGuiUpdateNeeded();
	}
}

void Match::AIUseCard(size_t index)
{
	assert(!isPlayersTurn());

	if (!AI_player_.playedCard()) {
		AI_player_.useCard(index);
		setGuiUpdateNeeded();
	}

}

void Match::AIEndTurn()
{
	assert(!isPlayersTurn());

	AI_player_.notifyTurnEnd();
	if (AI_player_.getScore() > ScoreLimit) {
		endSet();
	}
	else {
		if (!human_player_.stands()) { // If player stands repeat turn
			changeTurn();
		}
		newTurn();
	}
	setGuiUpdateNeeded();

}

void Match::AIStand()
{
	assert(!isPlayersTurn());

	AI_player_.notifyStand();
	if (AI_player_.getScore() > ScoreLimit) {
		endSet();
	}
	else {
		if (!human_player_.stands()) {
			changeTurn();
			newTurn();
		}
		else {
			endSet();
		}
	}
	setGuiUpdateNeeded();
}

void Match::newSet()
{
	human_player_.newSet();
	AI_player_.newSet();
	createNewDeck();
	flipWhoStarts(); // Change who begins each set
	player_turn_ = doesPlayerStart();
	newTurn();
	setGuiUpdateNeeded();
}

void Match::endSet()
{
	assert((human_player_.stands() && AI_player_.stands()) || 
		human_player_.getScore() > ScoreLimit || 
		AI_player_.getScore() > ScoreLimit);

	SetResult result;

	if (human_player_.getScore() > ScoreLimit || (isPlayersTurn() && !human_player_.canDrawCard() && !human_player_.stands())) { // Player overdrew or cannot draw
		result = SetResult::Loss;
	}
	else if (AI_player_.getScore() > ScoreLimit || (!isPlayersTurn() && !AI_player_.canDrawCard() && !AI_player_.stands())) { // AI overdrew or cannot draw
		result = SetResult::Win;
	}
	// Both players are under the ScoreLimit
	else if (human_player_.getScore() > AI_player_.getScore()) {
		result = SetResult::Win;
	}
	else if (human_player_.getScore() < AI_player_.getScore()) {
		result = SetResult::Loss;
	}
	else { // Players are tied
		if (human_player_.playedTiebraker() && AI_player_.playedTiebraker()) {
			result = SetResult::Draw;
		}
		else if (human_player_.playedTiebraker()) {
			result = SetResult::Win;
		}
		else if (AI_player_.playedTiebraker()) {
			result = SetResult::Loss;
		}
		else {
			result = SetResult::Draw;
		}
	}

	switch (result) {
	case SetResult::Win:
		human_player_.notifySetWon();
		AI_player_.notifySetLost();
		break;
	case SetResult::Loss:
		human_player_.notifySetLost();
		AI_player_.notifySetWon();
		break;
	case SetResult::Draw:
		human_player_.notifySetDraw();
		AI_player_.notifySetDraw();
		break;
	}

	// get the updated won sets 
	if (human_player_.getSetsWon() == MaxSets || AI_player_.getSetsWon() == MaxSets) {
		signalMatchEnd();
	}

	switch (result) {
	case SetResult::Win:
		parent_->notifySetWon();
		break;
	case SetResult::Loss:
		parent_->notifySetLost();
		break;
	case SetResult::Draw:
		parent_->notifySetDraw();
		break;
	}
}

void Match::signalMatchEnd()
{
	game_ongoing_ = false;
}
