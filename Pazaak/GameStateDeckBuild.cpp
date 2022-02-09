
#include <memory>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameStateDeckBuild.h"
#include "GameStateMatch.h"
#include "GuiCardCollection.h"
#include "GuiCard.h"
#include "Card.h"

GameStateDeckBuild::GameStateDeckBuild(Game* game, sf::RenderWindow* window, CardCollection&& available, CardCollection&& pre_selected) : 
	gui_{ game }, deck_build_{ std::move(available), std::move(pre_selected) }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

GameStateDeckBuild::GameStateDeckBuild(Game* game, sf::RenderWindow* window, const CardCollection& available, CardCollection&& pre_selected) :
	gui_{ game }, deck_build_{ available, std::move(pre_selected) }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

GameStateDeckBuild::GameStateDeckBuild(Game* game, sf::RenderWindow* window, CardCollection&& available, const CardCollection& pre_selected) :
	gui_{ game }, deck_build_{ std::move(available), pre_selected }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

GameStateDeckBuild::GameStateDeckBuild(Game* game, sf::RenderWindow* window, const CardCollection& available, const CardCollection& pre_selected) :
	gui_{ game }, deck_build_{ available, pre_selected }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);
}

void GameStateDeckBuild::update(const double dt)
{
	handleInput();
	gui_.update(deck_build_);
}

void GameStateDeckBuild::draw(const double dt)
{
	window_->setView(view_);

	window_->clear();
	window_->draw(game_->getBackground());
	window_->draw(gui_);
}

void GameStateDeckBuild::handleInput()
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
				if (gui_.clickedProceedButton(ev)) {
					loadMatch();
				}
				else if (auto opt = gui_.clickedAvailableIndex(ev)) {
					auto clicked_index = opt.value();
					deck_build_.add(clicked_index);
				}
				else if (auto opt = gui_.clickedSelectedIndex(ev)) {
					auto clicked_index = opt.value();
					deck_build_.remove(clicked_index);
				}
			}
			break;

		default:
			break;
		}
	}
}

void GameStateDeckBuild::loadMatch()
{
	if (game_->getCampaign().isActive()) {
		game_->changeState(std::make_unique<GameStateMatch>(game_, window_, GameConst::PlayerName, 
			game_->getCampaign().getNextOpponentName(), deck_build_.getSelectedCards(), 
			game_->getCampaign().getNextOpponentDeck()));
	}
	else {
		game_->changeState(std::make_unique<GameStateMatch>(game_, window_, GameConst::PlayerName, 
			GameConst::OpponentName, deck_build_.getSelectedCards(), game_->getFullCardCollection()));
	}
}

