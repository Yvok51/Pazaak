
#include <SFML/Graphics.hpp>
#include "GuiText.h"

GuiText::GuiText(const GuiStyle& style, unsigned int charSize)
{
	text_.setFont(style.getFont());
	text_.setFillColor(style.getTextCol());
	text_.setOutlineColor(style.getTextBorderCol());
	text_.setCharacterSize(charSize);
}
