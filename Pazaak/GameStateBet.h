
#include <array>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Counter.h"
#include "Game.h"
#include "GuiBet.h"

#ifndef GAMESTATEBET_H_
#define GAMESTATEBET_H_

/* Game state in which the player bets on the upcoming match */
class GameStateBet final : public GameState {
public:
	GameStateBet(Game* game, sf::RenderWindow* window);
	virtual void update(const double dt) override;
	virtual void draw(const double dt) override;

private:
	void handleInput();
	void exit() { window_->close(); }
	void proceedDeckBuild();
	
	static inline const std::array<int, 4> values_ = { 1, 10, 100, 1000 };

	GuiBet gui_;

	Counter bet_;
	int player_credits_;

	sf::View view_;

};

#endif // !GAMESTATEBET_H_
