
#include <random>
#include <algorithm>
#include <stdexcept>
#include "Deck.h"

Deck::Deck(const CardCollection& collection) : rand_(std::random_device{}())
{
	for (auto&& card : collection.collect_) {
		deck_.emplace_back(card->clone());
	}
	shuffle();
}

Deck& Deck::operator=(const Deck& deck)
{
	if (&deck != this) {
		deck_.clear();
		clone(deck);
	}
	return *this;
}

Deck& Deck::operator=(Deck&& deck) noexcept
{
	if (&deck != this) {
		deck_.clear();
		deck_ = std::move(deck.deck_);
	}
	return *this;
}

void Deck::add(std::unique_ptr<Card> card) {
	deck_.emplace_back(std::move(card));
}

/* Add a certain card a certain amount of times
*/
void Deck::add(std::unique_ptr<Card> card, size_t amount)
{
	if (amount != 0) {
		for (size_t i = 0; i < amount - 1; i++)
		{
			deck_.emplace_back(card->clone());
		}
		deck_.emplace_back(std::move(card));
	}
}

/* Draw a random card from the deck
* returns a unique_ptr to a card and deletes the card from the deck
*/
std::unique_ptr<Card> Deck::drawCard() {
	if (deck_.empty()) {
		throw std::out_of_range{ "Attempted to draw from an empty deck" };
	}
	auto card = deck_.back()->clone();
	deck_.pop_back();
	return card;
}

/* shoufle the cards */
void Deck::shuffle() {
	std::shuffle(deck_.begin(), deck_.end(), rand_);
}

void Deck::clone(const Deck& deck)
{
	for (auto&& card : deck.deck_) {
		deck_.emplace_back(card->clone());
	}
}
