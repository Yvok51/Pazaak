
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "CardCollection.h"
#include "GameState.h"
#include "GuiDeckBuild.h"
#include "DeckBuild.h"

#ifndef GAMESTATEDECKBUILD_H_
#define GAMESTATEDECKBUILD_H_

class GameStateDeckBuild final : public GameState {
public:
	GameStateDeckBuild(Game* game, sf::RenderWindow* window, CardCollection&& available, CardCollection&& pre_selected);
	GameStateDeckBuild(Game* game, sf::RenderWindow* window, const CardCollection& available, CardCollection&& pre_selected);
	GameStateDeckBuild(Game* game, sf::RenderWindow* window, CardCollection&& available, const CardCollection& pre_selected);
	GameStateDeckBuild(Game* game, sf::RenderWindow* window, const CardCollection& available, const CardCollection& pre_selected);
	virtual void update(const double dt) override;
	virtual void draw(const double dt) override;
private:
	void handleInput();
	void exit() { window_->close(); }
	void loadMatch();

	sf::View view_;
	
	GuiDeckBuild gui_;
	DeckBuild deck_build_;
};

#endif // !GAMESTATEDECKBUILD_H_

