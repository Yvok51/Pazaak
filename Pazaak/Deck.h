
#include <random>
#include <vector>
#include <memory>
#include "Card.h"
#include "CardCollection.h"

#ifndef DECK_H_
#define DECK_H_

/* Deck of cards
* Can be shuffled and afterwards the cards are drawn in a random order
*/
class Deck final {
public:
	using CardVec = std::vector<std::unique_ptr<Card>>;

	Deck() : rand_(std::random_device{}()) {};
	Deck(const Deck& deck) : rand_(std::random_device{}()) { clone(deck); }
	Deck(CardCollection&& collection) : rand_(std::random_device{}()), deck_(std::move(collection.collect_)) { shuffle(); };
	Deck(const CardCollection& collection);
	Deck(Deck&& deck) noexcept : rand_(std::move(deck.rand_)), deck_(std::move(deck.deck_)) {};
	Deck& operator=(const Deck& deck);
	Deck& operator=(Deck&& deck) noexcept;
	~Deck() = default;

	bool empty() const { return deck_.empty(); }

	// Modifiers
	void add(std::unique_ptr<Card> card);
	void add(std::unique_ptr<Card> card, size_t amount);
	std::unique_ptr<Card> drawCard();
	void clear() { deck_.clear(); }
	void shuffle();

private:
	void clone(const Deck& deck);

	std::default_random_engine rand_;
	CardVec deck_;
};

#endif // !DECK_H_
