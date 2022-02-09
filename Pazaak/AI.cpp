
#include <cassert>
#include "AI.h"
#include "Match.h"
#include "Player.h"

/**
* Perform the best move for the AI player in match
* Goes through possible situations the AI can be in.
*
* Step 1: Checks for direct win/lose situations.
* Step 2: Check if we have a usable card in hand and get the best one
* Step 3: Checks for necessity of using a card to not lose
* Step 4: Check if we can win if we use a card
* Step 5: We can't win by using a card -> should we use a card (draw)?
* Step 6: Nothing interesting to do -> endTurn
*
*/
void AI::performTurn(Match* match)
{
	auto our_score = match->getAIScore();
	auto enemy_score = match->getPlayerScore();

	// Step 1
	if (our_score == ScoreLimit) {
		match->AIStand(); // Cannot improve
		return;
	}
	else if (match->isPlayerStanding() && enemy_score < our_score && our_score <= ScoreLimit) {
		match->AIStand(); // We'll surely win
		return;
	}
	else if (match->playerPlayedTieBraker() && enemy_score == ScoreLimit) {
		match->AIStand(); // We lost nothing to do
		return;
	}

	// Step 2 - get the best card
	// new_score should be <= ScoreLimit
	auto [can_use_card, card_flipped, card_index, new_score] = getBestHandCard(match, our_score);

	assert(new_score <= ScoreLimit);

	if (!can_use_card) { // Can't use card without wasting it
		performTurnWithoutCard(match);
		return;
	}

	// Step 3 - Check whether we have to use a card
	if (our_score > ScoreLimit) { // Have to use card (and we know we can from previous step)
		if (enemy_score > new_score && enemy_score <= ScoreLimit && new_score >= HighMargin) {
			match->AIStand(); // Would probably lose anyway
		}
		useCard(match, card_index, card_flipped);
		performTurnWithoutCard(match);
		return;
	}

	// Step 4 - Can we win with the card

	if (match->isPlayerStanding() && enemy_score >= LowMargin && enemy_score < new_score && new_score <= ScoreLimit) {
		useCard(match, card_index, card_flipped); // known victory
		performTurnWithoutCard(match);
		return;
	}

	if (our_score > LowMargin && new_score > HighMargin && new_score <= ScoreLimit && enemy_score < new_score) {
		useCard(match, card_index, card_flipped); // favourable odds (we are close to ScoreLimit and have more)
		performTurnWithoutCard(match);
		return;
	}

	// Step 5: Should we use the card at all

	if (our_score >= HighMargin) {
		if (shouldWeDraw(match, new_score)) {
			useCard(match, card_index, card_flipped);
			performTurnWithoutCard(match);
		}
		else {
			if (enemy_score > our_score) {
				match->AIEndTurn(); // Gamble
			}
			else {
				match->AIStand();
			}
		}
		return;
	}
	else if (our_score >= LowMargin) {
		if (shouldWeDraw(match, new_score)) { // we should draw
			useCard(match, card_index, card_flipped);
			performTurnWithoutCard(match);
			return;
		}
	}

	match->AIEndTurn();

}

/* Use Card
* Use the given card 
* @param match - match to use the card in
* @param card_index - index of the AI player card in hand to use
* @param card_flipped - whether to flip the card before using it
*/
void AI::useCard(Match* match, int card_index, bool card_flipped)
{	
	assert(match->getAIHandCard(card_index).has_value());
	assert(match->getAIHandCard(card_index).value()->usable());

	if (card_flipped) {

		assert(match->getAIHandCard(card_index).value()->isFlipable());

		match->AIFlipHandCard(card_index);
	}

	match->AIUseCard(card_index);
}

/* Perform turn without card
* A simplified method which performs the best turn
* when the AI cannot use a card (no possible, already used it, ...)
*/
void AI::performTurnWithoutCard(Match* match)
{
	auto our_score = match->getAIScore();
	auto enemy_score = match->getPlayerScore();

	if (our_score == ScoreLimit) {
		match->AIStand(); // Cannot improve
		return;
	}
	else if (match->isPlayerStanding() && enemy_score < our_score) {
		match->AIStand(); // we'll win or we'll lose and can't do anything about it
		return;
	}
	else if (match->playerPlayedTieBraker() && enemy_score == ScoreLimit) {
		match->AIStand(); // We lost, nothing to do
		return;
	}
	else if (our_score >= HighMargin && our_score >= enemy_score) { // Our score is too high to risk it and we don't have to
		match->AIStand();
		return;
	}

	match->AIEndTurn();
}

/* Should we draw
* Decide whether we should use a card to draw
* @param match - match we are concerned with
* @param our_score - new score we'd get if we used card
*/
bool AI::shouldWeDraw(const Match* match, int our_score)
{
	return match->isPlayerStanding() &&
		match->getPlayerScore() == our_score && // draw
		!match->playerPlayedTieBraker() && // opponent doesn't win the draw
		our_score <= ScoreLimit && // should be unnecessary
		match->getPlayerScore() >= HighMargin; // is drawing reasonable
}

/* Get Best Hand Card
* Get the best card to use
* @oaram match - mathc we are concerned with
* @param our score - our current score
* @retval bool - can we use a card
* @retval bool - the best card is flipped
* @retval int - the index of the best card in hand
* @retval int - our score after we play the card
*/
std::tuple<bool, bool, int, int> AI::getBestHandCard(const Match* match, int our_score)
{
	bool can_use_card = false; // Can use card without wasting it
	bool best_card_flipped = false;
	int best_card_index = -1;
	int best_score = our_score <= ScoreLimit ? our_score : static_cast<int>(ScoreLimit) - 1000;
	/* If we are under the limit there is not much sense in lowering our score
	On the other hand if we are above the limit any new_score lower than the limit will be better ->
	We have a sufficently low score such that we can always improve if possible */

	for (int i = 0; i < MaxCardsInHand; i++)
	{
		if (auto opt = match->getAIHandCard(i)) {
			auto card = opt.value();
			if (card->usable()) {
				{
					int new_score = scoreAfterCardUsed(match, card, false);

					if (isNewScoreBetter(new_score, best_score)) { // we are better 
						can_use_card = true;
						best_score = new_score;
						best_card_flipped = false;
						best_card_index = i;
					}
				}

				if (card->isFlipable()) {
					int new_score = scoreAfterCardUsed(match, card, true);
					if (isNewScoreBetter(new_score, best_score)) { // we are better 
						can_use_card = true;
						best_score = new_score;
						best_card_flipped = true;
						best_card_index = i;
					}
				}
			}
		}
	}
	return {can_use_card, best_card_flipped, best_card_index, best_score};
}

/* Score after hand card used
* New score after we would use the given card
* @param match - match we are concerned with
* @param card - card we would use
* @flip - should we flip the card before using it
*/
int AI::scoreAfterCardUsed(const Match* match, const Card* card, bool flip)
{
	int new_score = 0;
	for (size_t i = 0; i < MaxCardsOnTable; i++)
	{
		if (auto opt = match->getAITableCard(i)) {
			auto card_value = opt.value()->getValue();
			switch (card->getSpecialEffect()) {
			case CardEffect::ThreeAndSix:
				new_score = (card_value != 3 && card_value != 6 && card_value != -3 && card_value != -6) ? new_score + card_value : new_score - card_value;
				break;
			case CardEffect::TwoAndFour:
				new_score = (card_value != 2 && card_value != 4 && card_value != -2 && card_value != -4) ? new_score + card_value : new_score - card_value;
				break;
			case CardEffect::Double:
				if (auto opt = match->getAITableCard(i + 1)) { // this is the last card (next one is not defined)
					new_score += card_value;
				}
				else {
					new_score += 2 * card_value;
				}
				break;
			default:
				new_score += card_value;
				break;
			}
		}
	}

	new_score = flip ? new_score + card->getValue() : new_score + card->getValue();
	return new_score;
}

/* Is new score better
* Whether the new score is better
*/
bool AI::isNewScoreBetter(int new_score, int old_score)
{
	if (new_score > ScoreLimit) {
		return false;
	}
	else if (new_score <= ScoreLimit && old_score > ScoreLimit) {
		return true;
	}
	else if (new_score > old_score && new_score <= ScoreLimit) { // both are under ScoreLimit
		return true;
	}
	return false; // both are under ScoreLimit but new_score <= old_score
}
