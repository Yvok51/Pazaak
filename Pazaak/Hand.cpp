
#include <stdexcept>
#include "Hand.h"

/* Returns whether given card is flipable
* if the index is outside the bounds throws std::out_of_range 
*/
bool Hand::isFlipable(size_t index) const
{
	if (index < collect_.size()) {
		return collect_[index]->isFlipable();
	}
	else {
		throw std::out_of_range{ "Attempted to access Card in Hand out of range" };
	}
}

/* Flips given card's value
* if the index is outside the bounds throws std::out_of_range,
* if the card is no flipable throws std::logic_error exception 
*/
void Hand::flipValue(size_t index)
{
	if (index < collect_.size() && collect_[index]->isFlipable()) {
		collect_[index]->flipValue(); 
	}
	else if (collect_[index]->isFlipable()) {
		throw std::logic_error{ "Attempted to flip Card in Hand which is not flipable" };
	}
	else {
		throw std::out_of_range{ "Attempted to flip Card value in Hand outside the bounds" };
	}
}
