
#include <stdexcept>
#include <vector>
#include <memory>
#include "Card.h"
#include "CardCollection.h"

CardCollection& CardCollection::operator=(const CardCollection& collection)
{
	if (&collection != this) {
		max_size_ = collection.max_size_;
		collect_.clear();
		clone(collection);
	}
	return *this;
}

CardCollection& CardCollection::operator=(CardCollection&& collection) noexcept
{
	if (&collection != this) {
		max_size_ = collection.max_size_;
		collect_.clear();
		collect_ = std::move(collection.collect_);
	}
	return *this;
}

/* Add card to the end, 
* if collection full throw std::logic_error exception 
*/
void CardCollection::add(std::unique_ptr<Card> card)
{
	if (!isFull()) { 
		collect_.emplace_back(std::move(card)); 
	}
	else {
		throw std::logic_error{ "Attempted to add to CardCollection when it is full" };
	}
}

/* Insert card in given index, 
* If index outside bounds throws std::out_of_range exception,
* if collection is full throws std::logic_error exception */
void CardCollection::insert(std::unique_ptr<Card> card, size_t index)
{
	if (index <= collect_.size() && !isFull()) {
		collect_.emplace(collect_.begin() + index, std::move(card));
	}
	else if (index > collect_.size()) {
		throw std::out_of_range{ "Attempted to insert into a CardCollection outside the range" };
	}
	else {
		throw std::logic_error{ "Attempted to insert into CardCollection when it is full" };
	}
}

/* Removes card at index, 
* if index is out of range throws std::out_of_range exception*/
void CardCollection::remove(size_t index)
{
	if (index < collect_.size()) {
		collect_.erase(collect_.begin() + index);
	}
	else {
		throw std::out_of_range{ "Attempted to remove Card from CardCollection outside the range" };
	}
}

/* Returns whether collections contains the given card */
bool CardCollection::contains(const Card& card) const
{
	for (auto&& card_collect : collect_) {
		if (*card_collect == card) {
			return true;
		}
	}

	return false;
}

void CardCollection::clone(const CardCollection& collection)
{
	for (auto&& card : collection.collect_) {
		collect_.emplace_back(card->clone());
	}
}
