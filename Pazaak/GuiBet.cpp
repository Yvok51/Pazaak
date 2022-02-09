
#include <string>
#include <array>
#include <SFML/Graphics.hpp>
#include "GuiBet.h"

GuiBet::GuiBet(const Game* game, int credits, const std::array<int, 4>& values) : 
	description_{ game->getStyle(TextStyle) }, credit_text_{ game->getStyle(TextStyle) },
	bet_text_{ game->getStyle(TextStyle) }, values_{ values }, proceed_but_{ ButtonSize, game->getStyle(ButtonStyle), "Proceed"}
{
	setupText(credits);
	setupButtons(game);
}

void GuiBet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(description_, states);
	target.draw(bet_text_, states);
	target.draw(credit_text_, states);
	
	for (auto&& button : add_buttons_) {
		target.draw(button, states);
	}

	for (auto&& button : sub_buttons_) {
		target.draw(button, states);
	}

	target.draw(proceed_but_, states);
}

void GuiBet::update(int new_bet_value)
{
	bet_text_.setText("Your bet: " + std::to_string(new_bet_value));
}

std::optional<int> GuiBet::clickedValue(const sf::Event& ev)
{
	for (size_t i = 0; i < add_buttons_.size(); i++)
	{
		if (add_buttons_[i].clicked(ev)) {
			return values_[i];
		}
	}

	for (size_t i = 0; i < sub_buttons_.size(); i++)
	{
		if (sub_buttons_[i].clicked(ev)) {
			return -values_[i];
		}
	}

	return std::nullopt;
}

bool GuiBet::proceedButtonClicked(const sf::Event& ev)
{
	return proceed_but_.clicked(ev);
}

void GuiBet::setupText(int credits)
{
	description_.setPosition(DescriptionStart);
	description_.setText("Place your bet");

	credit_text_.setPosition(CreditsStart);
	credit_text_.setText("Your credits: " + std::to_string(credits));

	bet_text_.setPosition(BetTextStart);
	bet_text_.setText("Your bet: 0");
}

void GuiBet::setupButtons(const Game* game)
{
	for (size_t i = 0; i < add_buttons_.size(); i++)
	{
		add_buttons_[i].setSize(ButtonSize);
		add_buttons_[i].setText(std::to_string(values_[i]));
		add_buttons_[i].setStyle(game->getStyle(ButtonStyle));

		add_buttons_[i].setPosition(ButtonStart.x + static_cast<float>(i) * (ButtonSize.x + ButtonPadding), ButtonStart.y);
	}

	for (size_t i = 0; i < sub_buttons_.size(); i++)
	{
		sub_buttons_[i].setStyle(game->getStyle(ButtonStyle));
		sub_buttons_[i].setSize(ButtonSize);
		sub_buttons_[i].setText(std::to_string( -values_[i] ));
		sub_buttons_[i].setStyle(game->getStyle(ButtonStyle));
		sub_buttons_[i].setPosition(ButtonStart.x + static_cast<float>(i) * (ButtonSize.x + ButtonPadding), ButtonStart.y + ButtonSize.y + ButtonPadding);
	}
	proceed_but_.setPosition(ProceedButtonStart);
}
