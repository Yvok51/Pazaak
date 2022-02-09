
#include <vector>
#include <memory>
#include "GameState.h"

#ifndef GAMESTATEHANDLER_H
#define GAMESTATEHANDLER_H

/* Handles the game states and the changing between them */
class GameStateHandler final {
	using StateVec = std::vector<std::unique_ptr<GameState>>;

public:
	GameStateHandler() : delete_previous_{ false }, delete_current_{ false } {}
	void pushState(std::unique_ptr<GameState> state);
	void popState();
	void changeState(std::unique_ptr<GameState> state);
	std::unique_ptr<GameState>& peekState();

private:
	void deletePrevious();
	void deleteCurrent();

	StateVec states_;
	/* Bool flags used to indicate we should delete the given state
	We cannot delete the states immediatly since the program itself 
	is still using the state.
	We have to delete them after we are done using them. 
	Logically this happens when the next state is asked for with
	peekState */
	bool delete_previous_;
	bool delete_current_;
};

#endif // !GAMESTATEHANDLER_H

