
#include <SFML/Graphics.hpp>
#include "GuiStyle.h"

#ifndef INDICATOR_H_
#define INDICATOR_H_

/* Class representing a simple GUI bulb,
which lights up and dims */
class BulbIndicator final : public sf::Drawable {
public:
	BulbIndicator() : lit_up_{ true } {};
	BulbIndicator(const GuiStyle& style, float radius = 20.f, size_t point_count = 50 );
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Setters
	void setStyle(const GuiStyle& style);
	void setRadius(float radius) { bulb_.setRadius(radius); }
	void setPointCount(size_t point_count) { bulb_.setPointCount(point_count); }
	void setPosition(float x, float y) { bulb_.setPosition(x, y); }
	void setPosition(sf::Vector2f position) { bulb_.setPosition(position); }

	// Functionality
	void dim();
	void lightup();

private:
	sf::CircleShape bulb_;
	bool lit_up_;
};

#endif // !INDICATOR_H_
