
#include <stdexcept>
#include <memory>
#include "Player.h"
#include "Card.h"
#include "Table.h"
#include "Hand.h"
#include "Deck.h"

Player::Player(const std::string& name, Deck&& sideDeck) : 
	setsWon_{ 0 }, played_card_{ false }, stands_{ false }, name_{ name }
{
	while (!hand_.isFull() && !sideDeck.empty()) {
		hand_.add(sideDeck.drawCard());
	}
}

void Player::drawCard(std::unique_ptr<Card> card)
{
	table_.add(std::move(card));
}

void Player::flipCard(size_t index)
{
	if (index < hand_.size()) {
		if (hand_.isFlipable(index)) {
			hand_.flipValue(index);
		}
	}
	else {
		throw std::out_of_range{ "Attempted to flip Player Card out of bounds of its hand" };
	}
}

void Player::useCard(size_t index)
{
	if (index < hand_.size() && !table_.isFull() && hand_[index]->usable() && !played_card_) {
		table_.add(hand_[index]->clone());
		hand_.remove(index);
		hand_.insert(std::make_unique<EmptyCard>(), index);
		played_card_ = true;
	}
	else if (index >= hand_.size()) {
		throw std::out_of_range{ "Attempted to use Player Card out of bounds of its hand" };
	}
}

void Player::notifySetLost()
{
}

void Player::notifySetDraw()
{
}

void Player::notifySetWon()
{
	setsWon_++;
}

void Player::notifyTurnEnd()
{
	played_card_ = false;
}

void Player::notifyStand()
{
	played_card_ = false;
	stands_ = true;
}

int Player::getScore() const
{
	return table_.getSum();
}

bool Player::playedTiebraker() const
{
	for (auto&& card : table_) {
		if (card->getSpecialEffect() == CardEffect::Tiebraker) {
			return true;
		}
	}

	return false;
}

void Player::newSet()
{
	table_.clear();
	played_card_ = false;
	stands_ = false;
}
