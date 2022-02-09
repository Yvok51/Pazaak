
#include <array>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Game.h"
#include "Button.h"
#include "Counter.h"
#include "GuiText.h"
#include "GuiBet.h"
#include "GameStateDeckBuild.h"
#include "GameStateBet.h"


GameStateBet::GameStateBet(Game* game, sf::RenderWindow* window) : player_credits_{ 0 }, gui_{ game, game->getCampaign().getCredits() }, bet_{0, game->getCampaign().getCredits()} {
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

void GameStateBet::update(const double dt)
{
	handleInput();
	gui_.update(bet_.getValue());
}

void GameStateBet::draw(const double dt)
{
	window_->setView(view_);

	window_->clear();
	window_->draw(game_->getBackground());
	window_->draw(gui_);
}

void GameStateBet::handleInput()
{
	sf::Event ev;
	while (this->window_->pollEvent(ev)) {
		switch (ev.type)
		{
		case sf::Event::Closed:
			exit();
			break;

		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape) {
				exit();
			}
			break;

		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				if (auto amount = gui_.clickedValue(ev)) {
					bet_.add(amount.value());
				}
				else if (gui_.proceedButtonClicked(ev)) {
					proceedDeckBuild();
				}
			}
			break;

		default:
			break;
		}
	}
}

/* Proceed to the deck build state */
void GameStateBet::proceedDeckBuild()
{
	game_->getCampaign().registerBet(bet_.getValue());
	game_->changeState(std::make_unique<GameStateDeckBuild>(game_, window_, game_->getCampaign().getPlayerDeck(), CardCollection{ GameConst::DeckSelectLimit }));
}
