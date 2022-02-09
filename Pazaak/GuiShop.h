
#include <optional>
#include <SFML/Graphics.hpp>
#include "GuiCardCollection.h"
#include "Button.h"
#include "GuiText.h"
#include "Game.h"
#include "GameConst.h"
#include "Shop.h"

#ifndef GUISHOP_H_
#define GUISHOP_H_

class GuiShop : public sf::Drawable {
public:
	GuiShop(const Game* game);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
	void update(const Shop& shop);

	void hidePrice() { show_price_ = false; }
	void showPrice() { show_price_ = true; }
	void setPrice(const std::string& price_text);

	std::optional<size_t> clickedOwnedIndex(const sf::Event& ev) const { return owned_.clickedIndex(ev); }
	std::optional<size_t> clickedShopIndex(const sf::Event& ev) const { return shop_.clickedIndex(ev); }
	std::optional<size_t> mouseOverOwnedIndex(const sf::Event& ev) const { return owned_.mouseOverIndex(ev); }
	std::optional<size_t> mouseOverShopIndex(const sf::Event& ev) const { return shop_.mouseOverIndex(ev); }
	bool clickedProceedButton(const sf::Event& ev) const { return proceed_but_.clicked(ev); }
	bool clickedMenuButton(const sf::Event& ev) const { return return_but_.clicked(ev); }

private:
	void setupCollections();
	void setupText();
	void setupButtons();

	void updateCredits(const Shop& shop);
	void updateCollections(const Shop& shop);

	bool show_price_;
	const Game* game_;

	inline static sf::Vector2f Size = GameConst::ScreenSize;
	
	// Text
	inline static const unsigned int TextCharSize = 50;
	inline static const std::string TextStyle = GameConst::TextStyle;

	inline static const sf::Vector2f PriceStart = { 50.f, 650.f };
	inline static const sf::Vector2f CreditsStart = { 50.f, 20.f };

	GuiText price_;
	GuiText credits_;

	// CardCollections
	inline static const unsigned int CardsPerLine = 5;
	inline static const unsigned int MaxNumOfCards = GameConst::TotalNumberOfCards;
	inline static const unsigned int CardLines = MaxNumOfCards % CardsPerLine == 0 ? MaxNumOfCards / CardsPerLine : (MaxNumOfCards / CardsPerLine) + 1;
	inline static const float CardPadding = 10;

	inline static const float ShopWidth = CardsPerLine * (GameConst::CardDimensions.x + CardPadding) + CardPadding;
	inline static const sf::Vector2f ShopStart = { CreditsStart.x, CreditsStart.y + static_cast<float>(TextCharSize) + CardPadding};
	inline static const sf::Vector2f OwnedStart = { Size.x - ShopStart.x - ShopWidth, ShopStart.y };


	GuiCardCollection shop_;
	GuiCardCollection owned_;

	// Button
	inline static const sf::Vector2f ButtonSize = { 200.f, 70.f };
	inline static const sf::Vector2f ProceedStart = { 0.5f * (ShopStart.x + ShopWidth + OwnedStart.x - ButtonSize.x), 575.f };
	inline static const sf::Vector2f ReturnStart = { ProceedStart.x, ProceedStart.y - (ButtonSize.y + 20) };

	Button proceed_but_;
	Button return_but_;
};

#endif // !GUISHOP_H_
