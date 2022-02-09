
#include "Campaign.h"
#include "CardCollection.h"
#include "Shop.h"

Shop::Shop(const Campaign* campaign, const CardCollection& shop, const CardCollection& player_deck) : 
	shop_{ shop }, owned_{ player_deck }, credits_{ campaign->getCredits() }, campaign_{ campaign }
{
}

void Shop::sell(size_t index)
{
	if (index < owned_.size()) {
		credits_ += campaign_->getCardPrice(*(owned_[index]));
		owned_.remove(index);
	}
}

void Shop::buy(size_t index)
{
	if (index < shop_.size()) {
		int price = campaign_->getCardPrice(*(shop_[index]));
		if (price <= credits_ && !owned_.contains(*(shop_[index]))) {
			credits_ -= campaign_->getCardPrice(*(shop_[index]));
			owned_.add(shop_[index]->clone());
		}
	}
}
