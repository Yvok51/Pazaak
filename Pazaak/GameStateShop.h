
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "GuiShop.h"
#include "CardCollection.h"
#include "Shop.h"

#ifndef GAMESTATESHOP_H_
#define GAMESTATESHOP_H_

class GameStateShop final : public GameState {
public:
	GameStateShop(Game* game, sf::RenderWindow* window, const CardCollection& shop, const CardCollection& player_deck);
	virtual void update(const double dt) override;
	virtual void draw(const double dt) override;

private:
	void handleInput();
	void exit() { window_->close(); }

	void proceedToBet();
	void returnToMenu();
	
	Shop shop_;

	GuiShop gui_;
	sf::View view_;

};

#endif // !GAMESTATESHOP_H_

