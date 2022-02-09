
#include <string>
#include <SFML/Graphics.hpp>
#include "GuiStyle.h"

#ifndef GUITEXT_H_
#define GUITEXT_H_

class GuiText final : public sf::Drawable {
public:
	GuiText(const GuiStyle& style, unsigned int charSize = 30);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override { target.draw(text_, states); };

	void setText(const std::string& text) { text_.setString(text); }
	void setPosition(float x, float y) { text_.setPosition(x, y); }
	void setPosition(const sf::Vector2f& position) { text_.setPosition(position); }

	const sf::String& getText() const { return text_.getString(); }
private:
	sf::Text text_;
};

#endif // !GUITEXT_H_

