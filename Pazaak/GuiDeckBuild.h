
#include <optional>
#include <SFML/Graphics.hpp>
#include "GuiCardCollection.h"
#include "Button.h"
#include "Game.h"
#include "GameConst.h"
#include "DeckBuild.h"

#ifndef GUIDECKBUILD_H_
#define GUIDECKBUILD_H_

class GuiDeckBuild final : public sf::Drawable {
public:
	GuiDeckBuild(const Game* game);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	void update(const DeckBuild& deck_build);

	std::optional<size_t> clickedSelectedIndex(const sf::Event& ev) { return gui_selected_.clickedIndex(ev); }
	std::optional<size_t> clickedAvailableIndex(const sf::Event& ev) { return gui_available_.clickedIndex(ev); }
	bool clickedProceedButton(const sf::Event& ev) { return procced_but_.clicked(ev); }

private:
	void setupButtons();
	void setupAvailable();
	void setupSelected();

	void updateSelected(const DeckBuild& deck_build);
	void updateAvailable(const DeckBuild& deck_build);

	const Game* game_;

	inline static const sf::Vector2f ButtonSize = { 300.f, 70.f };
	inline static const sf::Vector2f ButtonStart = { 550.f, 575.f };
	Button procced_but_;

	inline static const sf::Vector2f AvailableStart = { 300.f, 100.f };
	inline static const sf::Vector2f SelectedStart = { 1000.f, AvailableStart.y };
	inline static const float CardSpacing = 10.f;
	inline static const sf::Vector2f CardDimensions = GameConst::CardDimensions;

	inline static const unsigned int AvailableCardsPerLine = 6;
	inline static const unsigned int AvailableMaxNumOfCards = GameConst::TotalNumberOfCards;
	inline static const unsigned int AvailableCardLines = AvailableMaxNumOfCards % AvailableCardsPerLine == 0 ? 
		AvailableMaxNumOfCards / AvailableCardsPerLine : 
		(AvailableMaxNumOfCards / AvailableCardsPerLine) + 1;

	inline static const unsigned int SelectedCardsPerLine = 2;
	inline static const unsigned int SelectedMaxNumOfCards = GameConst::DeckSelectLimit;
	inline static const unsigned int SelectedCardLines = SelectedMaxNumOfCards % SelectedCardsPerLine == 0 ?
		SelectedMaxNumOfCards / SelectedCardsPerLine :
		(SelectedMaxNumOfCards / SelectedCardsPerLine) + 1;

	GuiCardCollection gui_available_;
	GuiCardCollection gui_selected_;
};

#endif // !GUIDECKBUILD_H

