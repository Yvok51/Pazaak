
#include <SFML/Graphics.hpp>
#include "GuiCard.h"
#include "GuiCardCollection.h"

GuiCardCollection::GuiCardCollection(const GuiStyle& style) : spacing_{ 10 }
{
	rect_.setFillColor(style.getBodyCol());
	rect_.setOutlineColor(style.getBorderCol());
	rect_.setOutlineThickness(style.getBorderSize());
}

GuiCardCollection::GuiCardCollection(const GuiStyle& style, sf::Vector2f card_dimensions, sf::Vector2f position, sf::Vector2f size, float spacing) :
	card_dimensions_(card_dimensions), position_(position), size_(size), spacing_(spacing)
{
	rect_.setFillColor(style.getBodyCol());
	rect_.setOutlineColor(style.getBorderCol());
	rect_.setOutlineThickness(style.getBorderSize());
}

void GuiCardCollection::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto&& card : collect_) {
		target.draw(card, states);
	}
	target.draw(rect_, states);
}

void GuiCardCollection::add(GuiCard&& card)
{
	card.setPosition(getCardPosition(collect_.size()));
	collect_.emplace_back(card);
}

void GuiCardCollection::add(const GuiCard& card)
{
	collect_.emplace_back(card);
	collect_.back().setPosition(getCardPosition(collect_.size() - 1));
}

const GuiCard& GuiCardCollection::operator[](size_t index) const
{
	return collect_[index];
}

const GuiCard& GuiCardCollection::at(size_t index) const
{
	return collect_.at(index);
}

GuiCard& GuiCardCollection::operator[](size_t index)
{
	return collect_[index];
}

GuiCard& GuiCardCollection::at(size_t index)
{
	return collect_.at(index);
}

void GuiCardCollection::remove(size_t index)
{
	if (index < collect_.size()) {
		collect_.erase(collect_.begin() + index);
		updatePositions(index);
	}
	else {
		throw std::out_of_range{ "Attempted to remove GuiCard from GuiCardCollection that is out of range" };
	}
}

bool GuiCardCollection::clicked(const sf::Event& ev) const
{
	for (size_t i = 0; i < collect_.size(); ++i) {
		if (collect_[i].clicked(ev)) {
			return true;
		}
	}
	return false;
}

std::optional<size_t> GuiCardCollection::clickedIndex(const sf::Event& ev) const
{
	for (size_t i = 0; i < collect_.size(); ++i) {
		if (collect_[i].clicked(ev)) {
			return i;
		}
	}
	return std::nullopt;
}

std::optional<size_t> GuiCardCollection::mouseOverIndex(const sf::Event& ev) const
{
	for (size_t i = 0; i < collect_.size(); ++i) {
		if (collect_[i].mouseOver(ev)) {
			return i;
		}
	}
	return std::nullopt;
}

float GuiCardCollection::getWidthToFitCards(size_t cards_per_line) const
{
	return static_cast<float>(cards_per_line) * (card_dimensions_.x + spacing_) + spacing_;
}

float GuiCardCollection::getHeightToFitCards(size_t num_of_lines) const
{
	return static_cast<float>(num_of_lines) * (card_dimensions_.y + spacing_) + spacing_;
}

sf::Vector2f GuiCardCollection::getCardPosition(size_t index)
{
	int cards_per_line = static_cast<int>((size_.x - spacing_) / (card_dimensions_.x + spacing_));
	cards_per_line = cards_per_line > 0 ? cards_per_line : 1;
	int lines_needed = static_cast<int>(index) / cards_per_line;
	int cards_in_final_line = index % cards_per_line;
	float pos_x = position_.x + spacing_ + cards_in_final_line * (spacing_ + card_dimensions_.x);
	float pos_y = position_.y + spacing_ + lines_needed * (spacing_ + card_dimensions_.y);
	return sf::Vector2f(pos_x, pos_y);
}

void GuiCardCollection::updatePositions(size_t from)
{
	for (auto it = collect_.begin() + from; it != collect_.end(); ++it, ++from) {
		it->setPosition(getCardPosition(from));
	}
}
