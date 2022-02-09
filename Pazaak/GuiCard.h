
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Game.h"

#ifndef GUICARD_H_
#define GUICARD_H_

class GuiCard final : public sf::Drawable {
public:
	GuiCard(const Card* card, const Game* game, bool clickable = false, bool hidden = false);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
	// update Card to a new text and/or texture
	void update(const Game* game, const Card* card);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& pos);
	void setHidden(bool hidden) { hidden_ = hidden; }

	bool mouseOver(const sf::Event& ev) const;
	bool clicked(const sf::Event& ev) const;
	void setClickable(bool clickable);

private:
	void setTexture(const Game* game, const Color& color);
	void setText(const Game* game, const std::string& text);
	void centerText(float position_x, float position_y);

	bool clickable_;
	bool hidden_;

	sf::Sprite sprite_;
	sf::Text text_;
};

#endif // !GUICARD_H_
