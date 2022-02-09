
#include <memory>
#include "GameStateMenu.h"
#include "GameState.h"
#include "GameStateShop.h"
#include "GameStateDeckBuild.h"
#include "Game.h"

GameStateMenu::GameStateMenu(Game* game, sf::RenderWindow* window) : gui_{ game }, paused_{ false },
	prompt_{ game->getStyle(GameConst::PromptStyle), PromptSize, 0.5f * (sf::Vector2f{window->getSize()} - PromptSize) }
{
	this->game_ = game;
	this->window_ = window;

	// set View size as that of the window and center it
	sf::Vector2f pos = game->getWindowCenter();
	view_.setSize(2.f * pos);
	view_.setCenter(pos);

	setupPrompt();
}

void GameStateMenu::draw(const double dt)
{
	window_->setView(view_);

	window_->clear();
	window_->draw(game_->getBackground());
	window_->draw(gui_);
	window_->draw(prompt_);

}

void GameStateMenu::update(const double dt)
{
	handleInput();
}

void GameStateMenu::handleInput()
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
				if (!paused()) {
					if (gui_.clickedExitkButton(ev)) {
						exit();
					}
					else if (gui_.clickedQuickButton(ev)) {
						loadQuickMatch();
					}
					else if (gui_.clickedStartButton(ev)) {
						loadNewCampaign();
					}
					else if (gui_.clickedContinueButton(ev)) {
						try {
							loadOldCampaign();
						}
						catch (const std::exception& e) {
							prompt_.show();
							pause();
						}
					}
				}

				if (prompt_.clicked(ev)) {
					prompt_.hide();
					unpause();
				}

			}
			break;

		default:
			break;
			}
	}
}

void GameStateMenu::loadQuickMatch()
{
	game_->changeState(std::make_unique<GameStateDeckBuild>(game_, window_, game_->getFullCardCollection(), CardCollection{GameConst::DeckSelectLimit}));
}

void GameStateMenu::loadNewCampaign()
{
	game_->getCampaign().reset();
	game_->getCampaign().activate();
	game_->changeState(std::make_unique<GameStateShop>(game_, window_, game_->getFullCardCollection(), game_->getCampaign().getPlayerDeck()));
}

/* Load old campaign
* If save file is not accessible then throws std::runtime_error
*/
void GameStateMenu::loadOldCampaign()
{
	game_->loadCampaign();
	game_->getCampaign().activate();
	game_->changeState(std::make_unique<GameStateShop>(game_, window_, game_->getFullCardCollection(), game_->getCampaign().getPlayerDeck()));
}

void GameStateMenu::exit()
{
	window_->close();
}

void GameStateMenu::setupPrompt()
{
	prompt_.setText("Unable to open save");
	prompt_.addButton(Button{ 0.25f * PromptSize, game_->getStyle(GameConst::ButtonStyle), "OK" });
	prompt_.hide();
}
