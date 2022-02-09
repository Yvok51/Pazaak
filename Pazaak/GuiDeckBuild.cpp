
#include <SFML/Graphics.hpp>
#include "GuiDeckBuild.h"
#include "Game.h"
#include "DeckBuild.h"

GuiDeckBuild::GuiDeckBuild(const Game* game) : 
	gui_available_{ game->getStyle(GuiCardCollection::Style) },
	gui_selected_{ game->getStyle(GuiCardCollection::Style) }, game_{ game },
	procced_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "Procced to Match" }
{
	setupAvailable();
	setupSelected();
	setupButtons();
}

void GuiDeckBuild::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(gui_available_, states);
	target.draw(gui_selected_, states);
	target.draw(procced_but_, states);
}

void GuiDeckBuild::update(const DeckBuild& deck_build)
{
	updateSelected(deck_build);
	updateAvailable(deck_build);
}

void GuiDeckBuild::setupButtons()
{
	procced_but_.setPosition(ButtonStart);
}

void GuiDeckBuild::setupAvailable()
{
	gui_available_.setPosition(AvailableStart);
	gui_available_.setCardDimensions(CardDimensions);
	gui_available_.setSpacing(CardSpacing);
	gui_available_.setSize(gui_available_.getWidthToFitCards(AvailableCardsPerLine), gui_available_.getHeightToFitCards(AvailableCardLines));
}

void GuiDeckBuild::setupSelected()
{
	gui_selected_.setPosition(SelectedStart);
	gui_selected_.setCardDimensions(CardDimensions);
	gui_selected_.setSpacing(CardSpacing);
	gui_selected_.setSize(gui_selected_.getWidthToFitCards(SelectedCardsPerLine), gui_selected_.getHeightToFitCards(SelectedCardLines));
}

void GuiDeckBuild::updateSelected(const DeckBuild& deck_build)
{
	for (int i = 0; i < SelectedMaxNumOfCards; i++)
	{
		if (auto card = deck_build.getSelectedCard(i)) {
			if (i < static_cast<int>(gui_selected_.size())) { // GUI Card is defined
				gui_selected_[i].update(game_, card.value());
			}
			else { // There are more selected cards than we show
				gui_selected_.add(GuiCard{ card.value(), game_, true });
			}
		}
		else { // No more selected cards -> delete the remaining GUI cards
			if (i < static_cast<int>(gui_selected_.size())) {
				gui_selected_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}
}

void GuiDeckBuild::updateAvailable(const DeckBuild& deck_build)
{
	for (int i = 0; i < AvailableMaxNumOfCards; i++)
	{
		if (auto card = deck_build.getAvailableCard(i)) {
			if (i < static_cast<int>(gui_available_.size())) { // GUI Card is defined
				gui_available_[i].update(game_, card.value());
			}
			else { // There are more available cards than we show
				gui_available_.add(GuiCard{ card.value(), game_, true });
			}
		}
		else { // No more available cards -> delete the remaining GUI cards
			if (i < static_cast<int>(gui_available_.size())) {
				gui_available_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}
}

