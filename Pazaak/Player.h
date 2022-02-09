
#include <optional>
#include <memory>
#include <string>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Table.h"
#include "MatchObserver.h"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player final : public MatchObserver {
public:
	Player(const std::string& name, Deck&& sideDeck);

	// Modifiers
	/* Adds card to hand, if cannot draw throws std::logic_error */
	void drawCard(std::unique_ptr<Card> card);
	/* Flips card in hand, if card is not flipable does nothing,
	if card is outside range throws std::out_of_range */
	void flipCard(size_t index);
	/* Uses card in hand, if cannot use card then does nothing,
	if index is out of bounds of the Player's hand throws std::out_of_range */
	void useCard(size_t index);

	// Notify about match state change
	virtual void notifySetLost() override;
	virtual void notifySetDraw() override;
	virtual void notifySetWon() override;
	void notifyTurnEnd();
	void notifyStand();

	void newSet(); // start new set

	// Getters

	/* Returns whether given Card in hand is flipable, 
	if index is out of range returns false */
	bool isHandCardFlipable(size_t index) const { return index < hand_.size() ? hand_.isFlipable(index) : false; }
	std::optional<const Card*> getHandCard(size_t index) const { return index < hand_.size() ? std::optional<const Card*>{hand_[index].get()} : std::nullopt; }
	std::optional<const Card*> getTableCard(size_t index) const { return index < table_.size() ? std::optional<const Card*>{table_[index].get()} : std::nullopt; }
	size_t getSetsWon() const { return setsWon_; }
	int getScore() const;
	const std::string& getName() const { return name_; }
	bool playedTiebraker() const;
	bool stands() const { return stands_; }
	bool playedCard() const { return played_card_; }
	bool canDrawCard() { return !table_.isFull(); }

private:

	bool played_card_;
	bool stands_;
	size_t setsWon_;
	std::string name_;
	Hand hand_;
	Table table_;
};

#endif //!PLAYER_H_