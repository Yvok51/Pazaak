
#include <memory>
#include <vector>
#include "Card.h"
#include "CardCollection.h"
#include "Game.h"

#ifndef HAND_H_
#define HAND_H_

class Hand final : public CardCollection {
public:
	Hand() : CardCollection{ MaxSize } {};
	Hand(size_t max_size) : CardCollection{ max_size } {};
	Hand(const Hand& hand) : CardCollection{ hand } {};
	Hand(Hand&& hand) noexcept : CardCollection{ std::move(hand) } {};
	Hand& operator=(const Hand& hand) { CardCollection::operator=(hand); return *this; }
	Hand& operator=(Hand&& hand) noexcept { CardCollection::operator=(std::move(hand)); return *this; }

	/* Returns whether given card is flipable, if the index is outside the bounds throws std::out_of_range */
	bool isFlipable(size_t index) const;
	/* Flips given card's value, if the index is outside the bounds throws std::out_of_range,
	if the card is no flipable throws std::logic_error exception */
	void flipValue(size_t index);

private:
	static inline const size_t MaxSize = GameConst::MaxCardsInHand;

};

#endif // !HAND_H_

