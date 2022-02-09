
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Button.h"

#ifndef GUIMENU_H
#define GUIMENU_H

class GuiMenu final : public sf::Drawable {
public:
	GuiMenu(const Game* game);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	bool clickedQuickButton(const sf::Event& ev) { return quick_match_but_.clicked(ev); }
	bool clickedStartButton(const sf::Event& ev) { return start_camp_but_.clicked(ev); }
	bool clickedContinueButton(const sf::Event& ev) { return cont_camp_but_.clicked(ev); }
	bool clickedExitkButton(const sf::Event& ev) { return exit_but_.clicked(ev); }

private:
	void setupButtons(const Game* game);

	inline static const sf::Vector2f ButtonSize = { 300, 70 };

	Button quick_match_but_;
	Button start_camp_but_;
	Button cont_camp_but_;
	Button exit_but_;
};

#endif // !GUIMENU_H

