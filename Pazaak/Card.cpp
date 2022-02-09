
#include <string>
#include "Card.h"


/* Deafult constructors are here for the cereal library
* They should not be used otherwise */
HandCard::HandCard()
{
	color_ = Color::Blue;
	value_ = 1;
	text_ = "1";
}

/* HandCard constructor 
Throws std::logic_error if value is outside the allowed range */
HandCard::HandCard(int value)
{
	color_ = Color::Blue;
	value_ = value;
	text_ = std::to_string(value);

	if (value < Min || value > Max || value == 0) {
		throw std::logic_error{ "Card value out of allowed bounds" };
	}
	if (value < 0) {
		color_ = Color::Red;
	}
}

/* Deafult constructors are here for the cereal library
* They should not be used otherwise */
DealerCard::DealerCard()
{
	color_ = Color::Green;
	value_ = 1;
	text_ = "1";
}


/* DealerCard constructor
Throws std::logic_error if value is outside the allowed range */
DealerCard::DealerCard(int value)
{
	color_ = Color::Green;
	value_ = value;
	text_ = std::to_string(value);

	if (value < Min || value > Max || value == 0) {
		throw std::logic_error{ "Card value out of allowed bounds" };
	}
}

/* Deafult constructors are here for the cereal library
* They should not be used otherwise */
FlipableCard::FlipableCard()
{
	color_ = Color::BlueAndRed;
	value_ = 1;
	text_ = "1";
}


/* FlipableCard constructor
Throws std::logic_error if value is outside the allowed range */
FlipableCard::FlipableCard(int value)
{
	color_ = Color::BlueAndRed;
	value_ = value;
	text_ = std::to_string(value);

	if (value < Min || value > Max || value == 0) {
		throw std::logic_error{ "Card value out of allowed bounds" };
	}
}

ThreeAndSixCard::ThreeAndSixCard()
{
	color_ = Color::Yellow;
	value_ = 0;
	text_ = "3&6";
}

TwoAndFourCard::TwoAndFourCard()
{
	color_ = Color::Yellow;
	value_ = 0;
	text_ = "2&4";
}

DoubleCard::DoubleCard()
{
	color_ = Color::Yellow;
	value_ = 0;
	text_ = "D";
}

TiebrakerCard::TiebrakerCard()
{
	color_ = Color::Yellow;
	value_ = 1;
	text_ = "+1T";
}

EmptyCard::EmptyCard()
{
	color_ = Color::None;
	value_ = 0;
	text_ = "";
}

bool Card::operator==(const Card& rhs) const
{
	return typeid(*this) == typeid(rhs) && isEqual(rhs); // Not pretty, but works
}
