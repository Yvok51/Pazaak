
#include <SFML/Graphics.hpp>
#include "Indicator.h"

BulbIndicator::BulbIndicator(const GuiStyle& style, float radius, size_t point_count) : lit_up_{ true }, bulb_{radius, point_count}
{
	setStyle(style);
}

void BulbIndicator::setStyle(const GuiStyle& style)
{
	bulb_.setFillColor(style.getBodyCol());
	bulb_.setOutlineColor(style.getBorderCol());
	bulb_.setOutlineThickness(style.getBorderSize());
}

void BulbIndicator::dim()
{
	if (lit_up_) {
		auto color = bulb_.getFillColor();
		color.a = 64;
		bulb_.setFillColor(color);
		lit_up_ = false;
	}
}

void BulbIndicator::lightup()
{
	if (!lit_up_) {
		auto color = bulb_.getFillColor();
		color.a = 255;
		bulb_.setFillColor(color);
		lit_up_ = true;
	}
}

void BulbIndicator::draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
{
	target.draw(bulb_, states);
}
