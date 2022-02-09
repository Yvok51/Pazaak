
#include <array>
#include <optional>
#include <SFML/Graphics.hpp>
#include "GuiText.h"
#include "Button.h"
#include "Game.h"
#include "GameConst.h"

#ifndef GUIBET_H_
#define GUIBET_H_

class GuiBet final : public sf::Drawable {
public:
	GuiBet(const Game* game, int credits, const std::array<int, 4>& values = { 1, 10, 100, 1000 });
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
	void update(int new_bet_value);

	std::optional<int> clickedValue(const sf::Event& ev);
	bool proceedButtonClicked(const sf::Event& ev);

private:
	void setupText(int credits);
	void setupButtons(const Game* game);

	static inline const sf::Vector2f Size = GameConst::ScreenSize;

	// Text
	static inline const std::string TextStyle = GameConst::TextStyle;
	static inline const float TextPadding = 20;
	static inline const unsigned int CharSize = 50;
	static inline const sf::Vector2f DescriptionStart = { 200, 100 };
	static inline const sf::Vector2f CreditsStart = { DescriptionStart.x, DescriptionStart.y + CharSize + TextPadding };
	static inline const sf::Vector2f BetTextStart = { DescriptionStart.x, DescriptionStart.y + 2.f * (CharSize + TextPadding) };

	GuiText description_;
	GuiText credit_text_;
	GuiText bet_text_;

	// Buttons
	const std::array<int, 4> values_;

	static inline const std::string ButtonStyle = GameConst::ButtonStyle;
	static inline const sf::Vector2f ButtonSize = { 150, 70 };
	static inline const float ButtonPadding = 10;
	static inline const sf::Vector2f ButtonStart = { 0.5f * (Size.x - 4 * (ButtonSize.x + ButtonPadding) + ButtonPadding), 400 };
	static inline const sf::Vector2f ProceedButtonStart = { 1000, 600 };

	std::array<Button, 4> add_buttons_;
	std::array<Button, 4> sub_buttons_;

	Button proceed_but_;

};

#endif // !GUIBET_H_
