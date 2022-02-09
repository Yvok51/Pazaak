
#include <string>
#include <SFML/Graphics.hpp>
#include "GuiStyle.h"

#ifndef BUTTON_H_
#define BUTTON_H_

/* Single color rectangular button with text
*  By default clickable
*/
class Button final : public sf::Drawable {
public:
	Button() : clickable_(true) {};
	Button(float width, float height, const GuiStyle& style, const std::string& text = "");
	Button(const sf::Vector2f& size, const GuiStyle& style, const std::string& text = "");
	// Override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	// Setters
	void setStyle(const GuiStyle& style);
	void setText(const std::string& text) { text_.setString(text); }
	void setCharSize(unsigned int size) { text_.setCharacterSize(size); }
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	void setSize(float width, float height);
	void setSize(const sf::Vector2f& size);
	void setClickable(bool clickable);

	// Getters
	sf::Vector2f getSize() const { return rect_.getSize(); }

	// User input
	bool mouseOver(const sf::Event& ev) const;
	bool clicked(const sf::Event& ev) const;

private:
	// Modifiers
	void setTransperent();
	void setOpaque();

	// Utility
	void centerText(float position_x, float position_y);
	unsigned int getCharSize(float button_height);

	bool clickable_;
	sf::RectangleShape rect_;
	sf::Text text_;
};

#endif // !BUTTON_H_
