
#include <vector>
#include <memory>
#include "Card.h"
#include "CardCollection.h"
#include "Game.h"

#ifndef TABLE_H_
#define TABLE_H_

/* Class represents the table during the match
* Applies the special effect when a card is added */
class Table final : public CardCollection {
public:
	using CardVec = std::vector<std::unique_ptr<Card>>;

	Table() : CardCollection{ MaxSize }, tiebraker_played_{ false } {};
	Table(size_t max_size) : CardCollection{ max_size }, tiebraker_played_{ false } {};
	Table(const Table& table) : CardCollection{ table }, tiebraker_played_{ false } {};
	Table(Table&& table) noexcept : CardCollection{ std::move(table) }, tiebraker_played_{ false } {};
	Table& operator=(const Table& table) { CardCollection::operator=(table); tiebraker_played_ = table.tiebraker_played_; return *this; };
	Table& operator=(Table&& table) noexcept { CardCollection::operator=(std::move(table)); tiebraker_played_ = table.tiebraker_played_; return *this; }

	/* Add Card to the back, if table is full throws std::logic_error exception */
	virtual void add(std::unique_ptr<Card> card) override;
	/* Insert Card to the specified index, if index is outside the bounds throws std::out_of_range exception, 
	if table is full throws an std::logic_error exception */
	virtual void insert(std::unique_ptr<Card> card, size_t index) override;

	int getSum() const;
private:
	void performSpecialAction(CardEffect effect);
	void flipAllOfGivenValue(int value);
	void doubleLast() { if (!collect_.empty()) { collect_.back()->doubleValue(); } }

	inline static constexpr size_t MaxSize = static_cast<size_t>(GameConst::MaxCardsOnTable);
	bool tiebraker_played_;
};

#endif // !TABLE_H_

