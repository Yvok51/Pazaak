
#include <optional>
#include "CardCollection.h"

#ifndef DECKBUILD_H_
#define DECKBUILD_H_

class DeckBuild {
public:
	DeckBuild(const CardCollection& available, const CardCollection& selected) : available_{ available }, selected_{ selected } {}
	DeckBuild(const CardCollection& available, CardCollection&& selected) : available_{ available }, selected_{ std::move(selected) } {}
	DeckBuild(CardCollection&& available, const CardCollection& selected) : available_{ std::move(available) }, selected_{ selected } {}
	DeckBuild(CardCollection&& available, CardCollection&& selected) : available_{ std::move(available) }, selected_{ std::move(selected) } {}

	// Functionality
	void add(size_t index);
	void remove(size_t index);

	// Getters
	const CardCollection& getSelectedCards() const { return selected_; }
	std::optional<const Card*> getAvailableCard(size_t index) const { return index < available_.size() ? std::optional<const Card*>{available_[index].get()} : std::nullopt; }
	std::optional<const Card*> getSelectedCard(size_t index) const { return index < selected_.size() ? std::optional<const Card*>{selected_[index].get()} : std::nullopt; }

private:

	CardCollection available_;
	CardCollection selected_;
};

#endif // !DECKBUILD_H_

