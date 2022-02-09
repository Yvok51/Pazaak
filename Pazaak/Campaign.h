
#include <string>

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>


#include "CardCollection.h"
#include "GameConst.h"
#include "Card.h"

#ifndef CAMPAIGN_H_
#define CAMPAIGN_H_

/* Stores information about the campaign
* Progress
* Player deck
* Credits
* Betting
*/
class Campaign final {
	friend class cereal::access;
public:
	Campaign() : owned_cards_{ GameConst::TotalNumberOfCards }, defeated_foes_{ 0 }, 
		credits_{ GameConst::CampaignStartCredits }, current_bet_{ 0 }, active_{ false }  {}

	void setCredits(int credits) { credits_ = credits; }
	void defeatedOpponent() { defeated_foes_++; }
	void setPlayerDeck(const CardCollection& deck) { owned_cards_ = deck; }
	void setPlayerDeck(CardCollection&& deck) { owned_cards_ = std::move(deck); }
	void addCardToDeck(std::unique_ptr<Card> card) { owned_cards_.add(std::move(card)); }
	void registerBet(int bet) { current_bet_ = bet; }
	void notifyMatchLost();
	void notifyMatchWon();
	void reset();
	void activate() { active_ = true; }
	void deactivate() { active_ = false; }

	std::string getNextOpponentName() const;
	CardCollection getNextOpponentDeck() const;
	const CardCollection& getPlayerDeck() const { return owned_cards_; }
	int getCredits() const { return credits_; }
	int getCardPrice(const Card& card) const;
	bool isActive() const { return active_; }
	bool isOver() const { return defeated_foes_ == GameConst::CampaignNumberOfEnemies; }

private:
	template <class	Archive>
	void serialize(Archive& archive); // Serialization method for cereal

	int getTotalCardValue();

	int current_bet_;

	int credits_;
	bool active_;
	int defeated_foes_;
	CardCollection owned_cards_;
};

template<class Archive>
inline void Campaign::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(current_bet_),
		CEREAL_NVP(credits_),
		CEREAL_NVP(active_),
		CEREAL_NVP(defeated_foes_),
		CEREAL_NVP(owned_cards_)
	);
}

#endif // !CAMPAIGN_H_
