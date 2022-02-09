
#include <SFML/Graphics.hpp>
#include "GameStateMatch.h"
#include "GameStateMenu.h"
#include "GameStateShop.h"
#include "CardCollection.h"
#include "Deck.h"
#include "Match.h"
#include "GuiMatch.h"
#include "Prompt.h"
#include "Button.h"

GameStateMatch::GameStateMatch(Game* game, sf::RenderWindow* window, const std::string& player_name, const std::string& opponent_name,
	CardCollection&& player_deck, CardCollection&& opponent_deck) : 
	match_{ game, this, player_name, opponent_name, Deck{ std::move(player_deck) }, Deck{ std::move(opponent_deck) } }, 
	gui_match_{ game }, timer_{ 0 }, prompt_{ game->getStyle(GameConst::PromptStyle), PromptSize, 0.5f * (sf::Vector2f{window->getSize()} - PromptSize) }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);

	setupPrompt();

}

GameStateMatch::GameStateMatch(Game* game, sf::RenderWindow* window, const std::string& player_name, const std::string& opponent_name, 
	const CardCollection& player_deck, CardCollection&& opponent_deck) :
	match_{ game, this, player_name, opponent_name, Deck{ player_deck }, Deck{ std::move(opponent_deck) } },
	gui_match_{ game }, timer_{ 0 }, prompt_{ game->getStyle(GameConst::PromptStyle), PromptSize, 0.5f * (sf::Vector2f{window->getSize()} - PromptSize) }
{
	game_ = game;
	window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = sf::Vector2f(this->window_->getSize());
	view_.setSize(pos);
	view_.setCenter(pos * 0.5f);

	setupPrompt();
}

void GameStateMatch::update(const double dt)
{
	handleInput();

	if (match_.isGuiUpdateNeeded()) {
		gui_match_.update(match_);
		match_.clearGuiUpdateNeeded();
	}

	if (!isPaused()) {
		if (!match_.isPlayersTurn()) {
			timer_ += dt;
			if (timer_ > AI::MinThinkingTime) {
				timer_ = 0;
				ai_.performTurn(&match_);
			}
		}
	}
}

void GameStateMatch::draw(const double dt)
{
	window_->setView(view_);
	window_->clear();

	window_->draw(game_->getBackground());
	window_->draw(gui_match_);
	window_->draw(prompt_);
}

/* Notify the state that player has lost to pop the appropriate prompt */
void GameStateMatch::notifySetLost()
{
	if (match_.isMatchOngoing()) {
		prompt_.setText("Set Lost!");
		prompt_.show();
	}
	else {
		prompt_.setText("Match Lost!");
		prompt_.show();
	}
	pause();
}

/* Notify the state that player has drawn to pop the appropriate prompt */
void GameStateMatch::notifySetDraw()
{
	if (match_.isMatchOngoing()) {
		prompt_.setText("Set Drawn!");
		prompt_.show();
	}
	else {
		throw std::runtime_error("Match ended with a draw");
	}
	pause();
}

/* Notify the state that player has won the set to pop the appropriate prompt */
void GameStateMatch::notifySetWon()
{
	if (match_.isMatchOngoing()) {
		prompt_.setText("Set Won!");
		prompt_.show();
	}
	else {
		prompt_.setText("Match Won!");
		prompt_.show();
	}
	pause();
}

void GameStateMatch::handleInput()
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
				if (match_.isPlayersTurn() && !isPaused()) {
					if (gui_match_.clickedEndTurnButton(ev)) {
						match_.playerEndTurn();
					}
					else if (gui_match_.clickedStandButton(ev)) {
						match_.playerStand();
					}
					else if (gui_match_.clickedForfeitButton(ev)) {
						match_.signalMatchEnd();
						notifySetLost();
					}
					{
						auto clicked_index = gui_match_.clickedFlipButtonIndex(ev);
						if (clicked_index) {
							match_.playerFlipHandCard(clicked_index.value());
						}
					}
					{
						auto clicked_index = gui_match_.clickedHandCardIndex(ev);
						if (clicked_index) {
							match_.playerUseCard(clicked_index.value());
						}
					}
				}

				else if (isPaused()) { // prompt is shown 
					if (prompt_.clicked(ev)) {
						unpause();
						if (match_.isMatchOngoing()) {
							match_.newSet();
							prompt_.hide();
						}
						else {
							endMatch();
							if (game_->getCampaign().isActive()) {
								continueInCampaign();
							}
							else {
								returnToMenu();
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

void GameStateMatch::returnToMenu()
{
	game_->changeState(std::make_unique<GameStateMenu>(game_, window_));
}

void GameStateMatch::continueInCampaign()
{
	if (match_.playerWon()) {
		game_->getCampaign().notifyMatchWon();
	}
	else {
		game_->getCampaign().notifyMatchLost();
	}
	if (game_->getCampaign().isOver()) {
		game_->getCampaign().deactivate();
		returnToMenu();

	}
	else {
		try {
			game_->saveCampaign();
		}
		catch (const std::exception& e) {
			std::cerr << "Unable to save campaign progress" << std::endl;
			std::cerr << e.what() << std::endl;
		}
		game_->changeState(std::make_unique<GameStateShop>(game_, window_, game_->getFullCardCollection(), game_->getCampaign().getPlayerDeck()));
	}
}

void GameStateMatch::endMatch()
{
}

void GameStateMatch::setupPrompt() 
{
	prompt_.addButton(Button{ 0.25f * PromptSize, game_->getStyle(GameConst::ButtonStyle), "OK" });
	prompt_.hide();
}
