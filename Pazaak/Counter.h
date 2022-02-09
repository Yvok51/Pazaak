
#include <limits>

#ifndef COUNTER_H_
#define COUNTER_H_

/*
* Simple counter to which you can add and
* which keeps check the value is in predifined bounds
*/
class Counter final {
public:
	Counter(int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) : value_{ 0 }, max_{ max }, min_{min} {}
	void add(int value);
	int getValue() const { return value_; }

private:
	int value_;
	int max_;
	int min_;
};

#endif // !COUNTER_H_
