
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AI.h"
#include "Match.h"
#include "GuiMatch.h"
#include "Game.h"
#include "Prompt.h"
#include "MatchObserver.h"

#ifndef GAMESTATEMATCH_H_
#define GAMESTATEMATCH_H_

/* Class representing te actual match */
class GameStateMatch final : public GameState, public MatchObserver {
public:
	GameStateMatch(Game* game, sf::RenderWindow* window, const std::string& player_name, const std::string& opponent_name, CardCollection&& player_deck, CardCollection&& opponent_deck);
	GameStateMatch(Game* game, sf::RenderWindow* window, const std::string& player_name, const std::string& opponent_name, const CardCollection& player_deck, CardCollection&& opponent_deck);
	virtual void update(const double dt) override;
	virtual void draw(const double dt) override;

	virtual void notifySetLost() override;
	virtual void notifySetDraw() override;
	virtual void notifySetWon() override;
private:
	void handleInput();
	void exit() { window_->close(); }
	void returnToMenu();
	void continueInCampaign();
	void endMatch();

	void setupPrompt();

	void pause() { match_paused_ = true; timer_ = 0; }
	void unpause() { match_paused_ = false; timer_ = 0; }
	bool isPaused() { return match_paused_; }

	static inline const sf::Vector2f PromptSize = { 250, 150 };

	double timer_; // Timer so that AI turns take some time

	AI ai_;
	Match match_;
	GuiMatch gui_match_;

	Prompt prompt_; // for announcing end of the set / match
	bool match_paused_;

	sf::View view_;

};

#endif // !GAMESTATEMATCH_H_

