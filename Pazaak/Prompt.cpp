
#include <optional>
#include <SFML/Graphics.hpp>
#include "Prompt.h"
#include "Button.h"

Prompt::Prompt(const GuiStyle& style, sf::Vector2f size, sf::Vector2f position, float padding, unsigned int char_size) :
	button_padding_{ padding }, container_{ size }, clickable_{ true }, hidden_{ false }
{
	setStyle(style);
	container_.setPosition(position);
	text_.setCharacterSize(char_size);
	reposition();
}

void Prompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden_) {
		target.draw(container_, states);
		for (auto&& button : buttons_) {
			target.draw(button, states);
		}
		target.draw(text_, states);
	}
}

void Prompt::setStyle(const GuiStyle& style)
{
	container_.setFillColor(style.getBodyCol());
	container_.setOutlineColor(style.getBorderCol());
	container_.setOutlineThickness(style.getBorderSize());

	text_.setFont(style.getFont());
	text_.setFillColor(style.getTextCol());
	text_.setOutlineColor(style.getBorderCol());
}

void Prompt::setSize(float x, float y)
{
	container_.setSize({x, y});
	reposition();
}

void Prompt::setSize(sf::Vector2f size)
{
	container_.setSize(size);
	reposition();
}

void Prompt::setPosition(float x, float y)
{
	container_.setPosition(x, y);
	reposition();
}

void Prompt::setPosition(sf::Vector2f position)
{
	container_.setPosition(position);
	reposition();
}

void Prompt::setButtonPadding(float padding)
{
	button_padding_ = padding;
	reposition();
}

void Prompt::setTextCharSize(unsigned int char_size)
{
	text_.setCharacterSize(char_size);
	reposition();
}

void Prompt::hide()
{
	hidden_ = true;
	clickable_ = false;
}

void Prompt::show()
{
	hidden_ = false;
	clickable_ = true;
}

void Prompt::addButton(const Button& button)
{
	buttons_.emplace_back(button);
	reposition();
}

void Prompt::addButton(Button&& button)
{
	buttons_.emplace_back(std::move(button));
	reposition();
}

void Prompt::setText(const std::string& text)
{
	text_.setString(text);
	reposition();
}

bool Prompt::clicked(const sf::Event& ev) const
{
	if (clickable_) {
		for (auto&& button : buttons_) {
			if (button.clicked(ev))
			{
				return true;
			}
		}
	}

	return false;
}

std::optional<size_t> Prompt::clickedIndex(const sf::Event& ev) const
{
	if (clickable_) {
		for (size_t i = 0; i < buttons_.size(); i++)
		{
			if (buttons_[i].clicked(ev)) {
				return i;
			}
		}
	}

	return std::nullopt;
}

void Prompt::reposition()
{
	auto center = getCenter();
	positionButtons();
	positionText();
}

void Prompt::positionButtons()
{
	auto next_button_start = getCenter() + sf::Vector2f{-0.5f * getButtonLineWidth(), button_padding_};
	for (auto&& button : buttons_) {
		button.setPosition(next_button_start);
		next_button_start.x += button.getSize().x + button_padding_; // move the next button over
	}
}

void Prompt::positionText()
{
	auto text_rect = text_.getLocalBounds();
	auto prompt_size = container_.getSize();
	auto position = container_.getPosition();
	// set origin point in the center of the text
	text_.setOrigin(text_rect.left + text_rect.width / 2.0f, text_rect.top + text_rect.height / 2.0f);
	// Set origin to the center offset a bit up
	text_.setPosition(position.x + prompt_size.x / 2.0f, (position.y + prompt_size.y / 2.0f) - (button_padding_ + 0.5f * static_cast<float>(text_.getCharacterSize())));
}

float Prompt::getButtonLineWidth() const
{
	float button_width = button_padding_;
	for (auto&& button : buttons_) {
		button_width += button.getSize().x + button_padding_;
	}
	return button_width;
}

sf::Vector2f Prompt::getCenter() const
{
	return container_.getPosition() + static_cast<float>(0.5) * container_.getSize();
}
