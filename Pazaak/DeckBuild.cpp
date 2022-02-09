#include "DeckBuild.h"

void DeckBuild::add(size_t index)
{
	if (index < available_.size()) {
		if (selected_.size() < selected_.getMaxSize()) {
			selected_.add(available_[index]->clone());
		}
	}
}

void DeckBuild::remove(size_t index)
{
	if (index < selected_.size()) {
		selected_.remove(index);
	}
}
