
#include "Card.h"
#include "CardCollection.h"
#include "GameConst.h"
#include "Campaign.h"

/* Notify match lost
* Notify the campaign the match was lost
*/
void Campaign::notifyMatchLost()
{
    credits_ -= current_bet_;
    current_bet_ = 0;
}

/* Notify match won
* Notify the campaign the match was won
*/
void Campaign::notifyMatchWon()
{
    credits_ += current_bet_ + 100;
    current_bet_ = 0;
    defeated_foes_++;
}

/* Reset the object to original state */
void Campaign::reset()
{ /* We could have rvalue assignment instead, but I'll be using this outside Game class 
  * which owns the campaign object and so I'll use reset method instead 
  * (though we have to keep it in sync with the constructor) */
    owned_cards_ = CardCollection{ GameConst::TotalNumberOfCards };
    defeated_foes_ = 0;
    credits_ = GameConst::CampaignStartCredits;
    current_bet_ = 0;
    active_ = false;
}

/* get the name of the next opponent */
std::string Campaign::getNextOpponentName() const
{
    switch (defeated_foes_)
    {
    case 0:
        return "Zaalbar";
        break;
    case 1:
        return "Mission";
        break;
    case 2:
        return "Carth";
        break;
    case 3:
        return "Bastila";
        break;
    default:
        return "Opponent";
        break;
    }
}

/* get the deck of the next opponent
* As the campaign progresses the decks get better
*/
CardCollection Campaign::getNextOpponentDeck() const
{
    const int HardOpponents = 2;
    const int MediumOpponents = 1;
    const int EasyOpponents = 0;

    CardCollection collection{ 22 };

    if (defeated_foes_ < HardOpponents) { // Hard opponents won't have any positive or negative numbers only flipable
        collection.add(std::make_unique<HandCard>(1));
        collection.add(std::make_unique<HandCard>(2));
        collection.add(std::make_unique<HandCard>(3));
        collection.add(std::make_unique<HandCard>(4));
        collection.add(std::make_unique<HandCard>(5));
        collection.add(std::make_unique<HandCard>(6));
    }

    if (defeated_foes_ > EasyOpponents && defeated_foes_ < HardOpponents) {
        collection.add(std::make_unique<HandCard>(-1));
        collection.add(std::make_unique<HandCard>(-2));
        collection.add(std::make_unique<HandCard>(-3));
        collection.add(std::make_unique<HandCard>(-4));
        collection.add(std::make_unique<HandCard>(-5));
        collection.add(std::make_unique<HandCard>(-6));
    }

    if (defeated_foes_ > MediumOpponents) {
        collection.add(std::make_unique<FlipableCard>(1));
        collection.add(std::make_unique<FlipableCard>(2));
        collection.add(std::make_unique<FlipableCard>(3));
        collection.add(std::make_unique<FlipableCard>(4));
        collection.add(std::make_unique<FlipableCard>(5));
        collection.add(std::make_unique<FlipableCard>(6));
    }

    return collection;
}

/* Get card price
* get the price of the given card
*/
int Campaign::getCardPrice(const Card& card) const
{
    const int Base = 100;
    int price = Base;

    if (card.getSpecialEffect() != CardEffect::None) { // special yellow cards
        price = 2 * price;
    }
    else { // other cards
        int modifier = card.getValue() < 0 ? 10 * (card.getValue() + 1) : 10 * -(card.getValue() - 1);
        price += modifier;
    }

    if (card.isFlipable()) { // Tiebreaker is maybe too expensive (400 credits)
        price = 2 * price;
    }
    
    return price;
}

/* Get the total value of the cards in the player deck
*/
int Campaign::getTotalCardValue()
{
    int total_card_price = 0;
    for (auto&& card : owned_cards_) {
        total_card_price += getCardPrice(*card);
    }
    return total_card_price;
}
