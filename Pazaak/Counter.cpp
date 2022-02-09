#include "Counter.h"

void Counter::add(int value)
{
	if (value_ + value >= min_ && value_ + value <= max_) {
		value_ += value;
	}
	else if (value_ + value < min_) {
		value_ = min_;
	}
	else if (value_ + value > max_) {
		value_ = max_;
	}
}
