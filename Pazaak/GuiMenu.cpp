#include "GuiMenu.h"

GuiMenu::GuiMenu(const Game* game) : 
	quick_match_but_(ButtonSize, game->getStyle(GameConst::ButtonStyle), "Quick Match"),
	start_camp_but_(ButtonSize, game->getStyle(GameConst::ButtonStyle), "Start Campaign"),
	cont_camp_but_(ButtonSize, game->getStyle(GameConst::ButtonStyle), "Continue Campaign"),
	exit_but_(ButtonSize, game->getStyle(GameConst::ButtonStyle), "Exit") 
{
	setupButtons(game);
}

void GuiMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(quick_match_but_, states);
	target.draw(start_camp_but_, states);
	target.draw(cont_camp_but_, states);
	target.draw(exit_but_, states);
}

void GuiMenu::setupButtons(const Game* game)
{
	auto center = game->getWindowCenter();
	center.x -= ButtonSize.x / 2;

	quick_match_but_.setPosition(center.x, center.y - ButtonSize.y * 3.5f);
	start_camp_but_.setPosition(center.x, center.y - ButtonSize.y * 1.5f);
	cont_camp_but_.setPosition(center.x, center.y + ButtonSize.y * 0.5f);
	exit_but_.setPosition(center.x, center.y + ButtonSize.y * 2.5f);
}
