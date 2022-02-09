
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GuiStyle.h"

#ifndef PROMPT_H_
#define PROMPT_H_

/* Prompt
* Prompt which contains a central text and buttons to interact
* It can be hidden
*/
class Prompt final : public sf::Drawable {
public:
	Prompt(const GuiStyle& style, sf::Vector2f size = { 300.f, 150.f }, sf::Vector2f position = {0.f, 0.f}, float padding = 10, unsigned int char_size = 30);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	// Setters
	void setStyle(const GuiStyle& style);
	void setSize(float x, float y);
	void setSize(sf::Vector2f size);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);
	void setButtonPadding(float padding);
	void setTextCharSize(unsigned int char_size);

	// Functionality
	void hide();
	void show();

	// Modifiers
	void addButton(const Button& button);
	void addButton(Button&& button);

	void setText(const std::string& text);

	// User Input
	bool clicked(const sf::Event& ev) const;
	std::optional<size_t> clickedIndex(const sf::Event& ev) const;

private:
	void reposition();
	void positionButtons();
	void positionText();
	sf::Vector2f getCenter() const;
	float getButtonLineWidth() const;

	float button_padding_;
	bool clickable_;
	bool hidden_;

	sf::Text text_;
	std::vector<Button> buttons_;
	sf::RectangleShape container_;
};

#endif // !PROMPT_H_
