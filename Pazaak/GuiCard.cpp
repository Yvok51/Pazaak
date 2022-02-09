
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Game.h"
#include "GuiStyle.h"
#include "GuiCard.h"

// include pointer to Game to get texture and font
GuiCard::GuiCard(const Card* card, const Game* game, bool clickable, bool hidden) : clickable_(clickable), hidden_(hidden)
{
	setTexture(game, card->getColor());
	setText(game, card->getText());
}

void GuiCard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
	if (!hidden_) {
		target.draw(text_, states);
	}
}

void GuiCard::update(const Game* game, const Card* card)
{
	if (text_.getString() != card->getText()) {
		text_.setString(card->getText());
	}

	setTexture(game, card->getColor());
}

void GuiCard::setPosition(float x, float y)
{
	sprite_.setPosition(x, y);
	centerText(x, y);
}

void GuiCard::setPosition(const sf::Vector2f& pos)
{
	setPosition(pos.x, pos.y);
}

bool GuiCard::mouseOver(const sf::Event& ev) const
{
	return sprite_.getGlobalBounds().contains(static_cast<float>(ev.mouseMove.x), static_cast<float>(ev.mouseMove.y));
}

bool GuiCard::clicked(const sf::Event& ev) const
{
	return clickable_ && sprite_.getGlobalBounds().contains(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
}

void GuiCard::setClickable(bool clickable)
{
	clickable_ = clickable;
}

void GuiCard::setTexture(const Game* game, const Color& color)
{
	if (color == Color::None) {
		sprite_.setTexture(game->getTexture("EmptyCard"));
	}
	else if (!hidden_) {
		switch (color) {
		case Color::Blue:
			sprite_.setTexture(game->getTexture("BlueCard"));
			break;
		case Color::BlueAndRed:
			sprite_.setTexture(game->getTexture("BlueAndRedCard"));
			break;
		case Color::Red:
			sprite_.setTexture(game->getTexture("RedCard"));
			break;
		case Color::Green:
			sprite_.setTexture(game->getTexture("GreenCard"));
			break;
		case Color::Yellow:
			sprite_.setTexture(game->getTexture("YellowCard"));
			break;
		}
	}
	else {
		sprite_.setTexture(game->getTexture("UnknownCard"));
	}
}

void GuiCard::setText(const Game* game, const std::string& text)
{
	const auto& style = game->getStyle(GameConst::CardStyle);
	text_.setFont(style.getFont());
	text_.setString(text);
	text_.setCharacterSize(GameConst::CardCharSize);
	text_.setFillColor(style.getTextCol());
}

void GuiCard::centerText(float position_x, float position_y)
{
	auto text_rect = text_.getLocalBounds();
	auto tex_size = sprite_.getTexture()->getSize();
	text_.setOrigin(text_rect.left + text_rect.width / 2.0f, text_rect.top + text_rect.height / 2.0f);
	text_.setPosition(position_x + static_cast<float>(tex_size.x) / 2.0f, position_y + static_cast<float>(tex_size.y) / 2.0f);
}
