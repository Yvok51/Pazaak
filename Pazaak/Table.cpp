
#include <vector>
#include <memory>
#include <stdexcept>
#include "Table.h"
#include "Card.h"

void Table::flipAllOfGivenValue(int value) {
	for (auto&& card : collect_) {
		if (card->getValue() == value || card->getValue() == -value) {
			card->flipValue();
		}
	}
}

void Table::add(std::unique_ptr<Card> card)
{
	if (!isFull()) {
		performSpecialAction(card->getSpecialEffect());
		card->updateText();
		collect_.emplace_back(std::move(card));
	}
	else {
		throw std::logic_error{ "Attempted to add card to Table when the Table is full" };
	}
}

void Table::insert(std::unique_ptr<Card> card, size_t index)
{
	if (index <= collect_.size() && !isFull()) {
		performSpecialAction(card->getSpecialEffect());
		card->updateText();
		collect_.emplace(collect_.begin() + index, std::move(card));
	}
	else if (index > collect_.size()) {
		throw std::out_of_range{ "Attempted to insert card into Table outside the range" };
	}
	else {
		throw std::logic_error{ "Attempted to insert card into Table when the Table is full" };
	}
}

int Table::getSum() const {
	int sum = 0;
	for (auto&& card : collect_) {
		sum += card->getValue();
	}
	return sum;
}


void Table::performSpecialAction(CardEffect effect)
{
	switch (effect) {
	case CardEffect::Double:
		doubleLast();
		break;

	case CardEffect::TwoAndFour:
		flipAllOfGivenValue(2);
		flipAllOfGivenValue(4);
		break;
	
	case CardEffect::ThreeAndSix:
		flipAllOfGivenValue(3);
		flipAllOfGivenValue(6);
		break;

	case CardEffect::Tiebraker:
		tiebraker_played_ = true;
		break;

	case CardEffect::None:
	default:
		break;
	}
}