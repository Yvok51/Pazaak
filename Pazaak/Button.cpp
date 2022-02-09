
#include <SFML/Graphics.hpp>
#include "Button.h"

Button::Button(float width, float height, const GuiStyle& style, const std::string& text) :
	rect_(sf::Vector2f(width, height)), text_(text, style.getFont(), getCharSize(height)),
	clickable_(true)
{
	rect_.setFillColor(style.getBodyCol());
	rect_.setOutlineColor(style.getBorderCol());
	rect_.setOutlineThickness(style.getBorderSize());
	text_.setFillColor(style.getTextCol());
}

Button::Button(const sf::Vector2f& size, const GuiStyle& style, const std::string& text) :
	rect_(size), text_(text, style.getFont(), getCharSize(size.y)), clickable_(true)
{
	rect_.setFillColor(style.getBodyCol());
	rect_.setOutlineColor(style.getBorderCol());
	rect_.setOutlineThickness(style.getBorderSize());
	text_.setFillColor(style.getTextCol());
}

/* Set the style of the button
* @param style - style to set the button to
*/
void Button::setStyle(const GuiStyle& style)
{
	rect_.setFillColor(style.getBodyCol());
	rect_.setOutlineColor(style.getBorderCol());
	rect_.setOutlineThickness(style.getBorderSize());
	text_.setFillColor(style.getTextCol());
	text_.setFont(style.getFont());
}

/* Set a new position
*/
void Button::setPosition(float x, float y)
{
	rect_.setPosition(x, y);
	centerText(x, y);
}

/* Set a new position
*/
void Button::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

/* Set a new size
* Button will automaticly readjust the text
*/
void Button::setSize(float width, float height)
{
	rect_.setSize(sf::Vector2f(width, height)); 
	text_.setCharacterSize(getCharSize(height));
	auto rect_position = rect_.getPosition();
	centerText(rect_position.x, rect_position.y);
}

/* Set a new size
* Button will automaticly readjust the text
*/
void Button::setSize(const sf::Vector2f& size)
{
	setSize(size.x, size.y);
}

/* Set clickable
* Set whether the button is clickable 
* by default true - is clickable
*/
void Button::setClickable(bool clickable)
{
	if (!clickable_ && clickable) {
		setOpaque();
	}
	else if (clickable_ && !clickable) {
		setTransperent();
	}
	clickable_ = clickable;
}

/* Set the button to be transparent
* signifies the button is UNclickable
*/
void Button::setTransperent()
{
	auto color = rect_.getFillColor();
	auto text_color = text_.getFillColor();
	color.a = 64;
	text_color.a = 64;
	rect_.setFillColor(color);
	text_.setFillColor(text_color);
}

/* Set the button to be opaque
* signifies the button is Clickable
*/
void Button::setOpaque()
{
	sf::Color color = rect_.getFillColor();
	auto text_color = text_.getFillColor();
	color.a = 255;
	text_color.a = 64;
	rect_.setFillColor(color);
	text_.setFillColor(text_color);
}

/* Center text
* @param position_x - new position of the button
* @param position_y - new position of the button
*/
void Button::centerText(float position_x, float position_y)
{
	auto text_rect = text_.getLocalBounds();
	auto button_size = rect_.getSize();
	text_.setOrigin(text_rect.left + text_rect.width / 2.0f, text_rect.top + text_rect.height / 2.0f);
	text_.setPosition(position_x + button_size.x / 2.0f, position_y + button_size.y / 2.0f);
}

/* Get char size
* Get the appropriate character size for the button size
*/
unsigned int Button::getCharSize(float button_height)
{
	return static_cast<unsigned int>(button_height / 2.0f);
}

/* Draw */
void Button::draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
{
	target.draw(rect_, states);
	target.draw(text_, states);
}

/* Mouse over
* Whehter the button was moused over
*/
bool Button::mouseOver(const sf::Event& ev) const
{
	return rect_.getGlobalBounds().contains(static_cast<float>(ev.mouseMove.x), static_cast<float>(ev.mouseMove.y));
}

/* Clicked
* Whehter the button was clicked
*/
bool Button::clicked(const sf::Event& ev) const
{
	return clickable_ && rect_.getGlobalBounds().contains(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
}
