
#include <vector>
#include <memory>
#include "GameStateHandler.h"

void GameStateHandler::pushState(std::unique_ptr<GameState> state)
{
    states_.emplace_back(std::move(state));
}

void GameStateHandler::popState()
{
    if (!states_.empty()) {
        delete_current_ = true;
    }
}

/* Change the current game state
*  Removes the last state */
void GameStateHandler::changeState(std::unique_ptr<GameState> state)
{
    if (!states_.empty()) {
        delete_previous_ = true;
    }
    pushState(std::move(state));
}

/* Get the current state
* Throws std::logic_error when there is no state */
std::unique_ptr<GameState>& GameStateHandler::peekState()
{
    if (delete_previous_) {
        deletePrevious();
    }
    if (delete_current_) {
        deleteCurrent();
    }
    if (states_.empty()) {
        throw std::logic_error{ "Tried to peek an empty game state stack" };
    }
    return states_.back();
}

void GameStateHandler::deletePrevious()
{
    states_.erase(states_.begin() + (states_.size() - 2));
    delete_previous_ = false;
}

void GameStateHandler::deleteCurrent()
{
    states_.pop_back();
    delete_current_ = false;
}
