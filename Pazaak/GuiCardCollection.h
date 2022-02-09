
#include <optional>
#include <string>
#include <SFML/Graphics.hpp>
#include "GuiCard.h"
#include "GuiStyle.h"
#include "Game.h"

#ifndef GUICARDCOLLECTION_H_
#define	GUICARDCOLLECTION_H_

class GuiCardCollection final : public sf::Drawable {
public:
	using Collection = std::vector<GuiCard>;

	GuiCardCollection(const GuiStyle& style);
	GuiCardCollection(const GuiStyle& style, sf::Vector2f card_dimensions, sf::Vector2f position, sf::Vector2f size = {0, 0}, float spacing = 10);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	// Modifiers
	void add(GuiCard&& card);
	void add(const GuiCard& card);
	/* Removes card from collection
	* If index is outside throws std::out_of_range
	*/
	void remove(size_t index);

	void setPosition(sf::Vector2f position) { position_ = position; updatePositions(); rect_.setPosition(position_); }
	void setPosition(float x, float y) { position_ = sf::Vector2f{ x, y }; updatePositions(); rect_.setPosition(position_); }
	void setSize(sf::Vector2f size) { size_ = size; updatePositions(); rect_.setSize(size_); }
	void setSize(float x, float y) { size_ = sf::Vector2f{ x, y }; updatePositions(); rect_.setSize(size_); }
	void setCardDimensions(sf::Vector2f dimensions) { card_dimensions_ = dimensions; updatePositions(); }
	void setCardDimensions(float x, float y) { card_dimensions_ = sf::Vector2f(x, y); updatePositions();}
	void setSpacing(float spacing) { spacing_ = spacing; updatePositions(); }
	
	// Accessors
	const GuiCard& operator[](size_t index) const;
	const GuiCard& at(size_t index) const;
	GuiCard& operator[](size_t index);
	GuiCard& at(size_t index);

	Collection::iterator begin() { return collect_.begin(); }
	Collection::iterator end() { return collect_.end(); }
	Collection::const_iterator begin() const { return collect_.begin(); }
	Collection::const_iterator end() const { return collect_.end(); }
	Collection::const_iterator cbegin() const { return collect_.cbegin(); }
	Collection::const_iterator cend() const { return collect_.cend(); }
	
	// Utility
	bool clicked(const sf::Event& ev) const;
	std::optional<size_t> clickedIndex(const sf::Event& ev) const;
	std::optional<size_t> mouseOverIndex(const sf::Event& ev) const;

	size_t size() const { return collect_.size(); }
	float getWidthToFitCards(size_t cards_per_line) const;
	float getHeightToFitCards(size_t num_of_lines) const;

	inline static std::string Style = GameConst::TransparentRectStyle;

private:
	sf::Vector2f getCardPosition(size_t index);
	void updatePositions(size_t from = 0);

	sf::RectangleShape rect_;

	float spacing_;
	sf::Vector2f card_dimensions_;
	sf::Vector2f position_;
	sf::Vector2f size_;
	std::vector<GuiCard> collect_;
};

#endif // !GUICARDCOLLECTION_H_

