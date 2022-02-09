
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameStateShop.h"
#include "GameStateBet.h"
#include "GameStateMenu.h"

GameStateShop::GameStateShop(Game* game, sf::RenderWindow* window, const CardCollection& shop, const CardCollection& player_deck) :
	shop_{ &(game->getCampaign()), shop, player_deck }, gui_{ game }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

void GameStateShop::update(const double dt)
{
	handleInput();
	gui_.update(shop_);
}

void GameStateShop::draw(const double dt)
{
	window_->setView(view_);

	window_->clear();
	window_->draw(game_->getBackground());

	window_->draw(gui_);
}

void GameStateShop::handleInput()
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
				if (auto index = gui_.clickedShopIndex(ev)) {
					shop_.buy(index.value());
				}
				else if (auto index = gui_.clickedOwnedIndex(ev)) {
					shop_.sell(index.value());
				}
				else if (gui_.clickedProceedButton(ev)) {
					proceedToBet();
				}
				else if (gui_.clickedMenuButton(ev)) {
					returnToMenu();
				}
			}
			break;

		case sf::Event::MouseMoved:
			if (auto index = gui_.mouseOverShopIndex(ev)) { // Get how much moused over card costs and set the price string appropriately
				gui_.setPrice("Buy for: " + std::to_string(game_->getCampaign().getCardPrice(*(shop_.getShopCard(index.value()).value()))));
				gui_.showPrice();
			}
			else if (auto index = gui_.mouseOverOwnedIndex(ev)) {
				gui_.setPrice("Sell for: " + std::to_string(game_->getCampaign().getCardPrice(*(shop_.getOwnedCard(index.value()).value()))));
				gui_.showPrice();
			}
			else {
				gui_.hidePrice();
			}
			break;

		default:
			break;
		}
	}
}

void GameStateShop::proceedToBet()
{
	game_->getCampaign().setCredits(shop_.getPlayerCredits()); // update player credits
	game_->getCampaign().setPlayerDeck(shop_.getOwnedCards()); // update player deck
	game_->changeState(std::make_unique<GameStateBet>(game_, window_));
}

void GameStateShop::returnToMenu()
{
	game_->getCampaign().deactivate();
	game_->changeState(std::make_unique<GameStateMenu>(game_, window_));
}
