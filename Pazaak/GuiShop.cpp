
#include "GuiShop.h"
#include "GuiCardCollection.h"
#include "Button.h"
#include "GuiText.h"
#include "GameConst.h"
#include "Shop.h"

GuiShop::GuiShop(const Game* game) : owned_{ game->getStyle(GuiCardCollection::Style) },
	shop_{ game->getStyle(GuiCardCollection::Style) }, price_{ game->getStyle(TextStyle), TextCharSize },
	credits_{ game->getStyle(TextStyle), TextCharSize }, show_price_{ false }, game_{ game },
	proceed_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "Proceed" },
	return_but_{ ButtonSize, game->getStyle(GameConst::ButtonStyle), "Main Menu" }
{
	setupCollections();
	setupText();
	setupButtons();
}

void GuiShop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(owned_);
	target.draw(shop_);
	target.draw(credits_);
	if (show_price_) {
		target.draw(price_);
	}
	target.draw(proceed_but_);
	target.draw(return_but_);
}

void GuiShop::update(const Shop& shop)
{
	updateCredits(shop);
	updateCollections(shop);
}

void GuiShop::setPrice(const std::string& price_text)
{
	price_.setText(price_text);
}

void GuiShop::setupCollections()
{
	shop_.setPosition(ShopStart);
	shop_.setCardDimensions(GameConst::CardDimensions);
	shop_.setSize(shop_.getWidthToFitCards(CardsPerLine), shop_.getHeightToFitCards(CardLines));
	shop_.setSpacing(CardPadding);

	owned_.setPosition(OwnedStart);
	owned_.setCardDimensions(GameConst::CardDimensions);
	owned_.setSize(owned_.getWidthToFitCards(CardsPerLine), owned_.getHeightToFitCards(CardLines));
	owned_.setSpacing(CardPadding);
}

void GuiShop::setupText()
{
	credits_.setPosition(CreditsStart);
	price_.setPosition(PriceStart);
}

void GuiShop::setupButtons()
{
	proceed_but_.setPosition(ProceedStart);
	return_but_.setPosition(ReturnStart);
}

void GuiShop::updateCredits(const Shop& shop)
{
	credits_.setText("Your credits: " + std::to_string(shop.getPlayerCredits()));
}

void GuiShop::updateCollections(const Shop& shop)
{
	for (int i = 0; i < MaxNumOfCards; i++)
	{
		if (auto card = shop.getShopCard(i)) {
			if (i < static_cast<int>(shop_.size())) { // GUI Card is defined
				shop_[i].update(game_, card.value());
			}
			else { // Shop has more cards than we
				shop_.add(GuiCard{ card.value(), game_, true });
			}
		}
		else { // No more cards in the logical shop -> delete the remaining GUI cards
			if (i < static_cast<int>(shop_.size())) {
				shop_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}

	for (int i = 0; i < MaxNumOfCards; i++)
	{
		if (auto card = shop.getOwnedCard(i)) {
			if (i < static_cast<int>(owned_.size())) { // GUI Card is defined
				owned_[i].update(game_, card.value());
			}
			else { // There are more owned card than we have
				owned_.add(GuiCard{ card.value(), game_, true });
			}
		}
		else { // No more cards owned -> delete the remaining GUI cards
			if (i < static_cast<int>(owned_.size())) {
				owned_.remove(i);
				--i; // We removed a card -> have to go back
			}
		}
	}
}
