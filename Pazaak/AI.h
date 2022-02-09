
#include "GameConst.h"
#include "Match.h"
#include "Player.h"

#ifndef AI_H_
#define AI_H_

class AI final {
public:
	void performTurn(Match* match);
	inline static const float MinThinkingTime = 1.f; // in seconds
private:
	void useCard(Match* match, int card_index, bool card_flipped);
	void performTurnWithoutCard(Match* match);
	bool shouldWeDraw(const Match* match, int our_score);
	std::tuple<bool, bool, int, int> getBestHandCard(const Match* match, int our_score);
	int scoreAfterCardUsed(const Match* match, const Card* card, bool flip);
	bool isNewScoreBetter(int new_score, int old_score);

	inline static const int ScoreLimit = GameConst::ScoreLimit;
	inline static const int MaxCardsInHand = GameConst::MaxCardsInHand;
	inline static const int MaxCardsOnTable = GameConst::MaxCardsOnTable;
	inline static const int HighMargin = GameConst::ScoreLimit - 2; // the more we substract the safer the AI will play
	inline static const int LowMargin = GameConst::ScoreLimit - 6; // the more we substract the safer the AI will play
};

#endif // !AI_H_
