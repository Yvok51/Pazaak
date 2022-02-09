
#include <optional>
#include <memory>
#include "CardCollection.h"
#include "Campaign.h"
#include "Card.h"

#ifndef SHOP_H_
#define SHOP_H_

/* Class representing a shop
* It has the available cards
* Gets price information from the Campaign
* Player can buy and sell cards
*/
class Shop final {
public:
	Shop(const Campaign* campaign, const CardCollection& shop, const CardCollection& player_deck);

	// Functionalty
	void sell(size_t index);
	void buy(size_t index);

	// Getters
	const CardCollection& getOwnedCards() const { return owned_; }
	std::optional<const Card*> getShopCard(size_t index) const { return index < shop_.size() ? std::optional<const Card*>{shop_[index].get()} : std::nullopt; }
	std::optional<const Card*> getOwnedCard(size_t index) const { return index < owned_.size() ? std::optional<const Card*>{owned_[index].get()} : std::nullopt; }

	int getCardPrice(const Card& card) const { return campaign_->getCardPrice(card); };
	int getPlayerCredits() const { return credits_; }
private:

	int credits_;
	const Campaign* campaign_;
	CardCollection shop_;
	CardCollection owned_;
};

#endif // !SHOP_H_
