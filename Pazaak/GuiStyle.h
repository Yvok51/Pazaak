
#include <SFML/Graphics.hpp>

#ifndef GUISTYLE_H_
#define GUISTYLE_H_

// Immutable class defining Gui style (colors & font)
class GuiStyle final {
public:
    GuiStyle(const sf::Font* font, float borderSize, const sf::Color& bodyCol,
        const sf::Color& borderCol, const sf::Color& textCol, const sf::Color& textBorderCol, const sf::Color& bodyHighlightCol,
        const sf::Color& borderHighlightCol, const sf::Color& textHighlightCol) :
        body_col_{ bodyCol }, body_highlight_col_{ bodyHighlightCol }, border_col_{ borderCol },
        border_highlight_col_{ borderHighlightCol }, text_col_{ textCol }, text_border_col_{ textBorderCol },
        text_highlight_col_{ textHighlightCol }, font_{ font }, border_size_{ borderSize } {};

    const sf::Color& getBodyCol() const { return body_col_; }
    const sf::Color& getBodyHighlightCol() const { return body_highlight_col_; }
    const sf::Color& getBorderCol() const { return border_col_; }
    const sf::Color& getBorderHighlightCol() const { return border_highlight_col_; }
    const sf::Color& getTextCol() const { return text_col_; }
    const sf::Color& getTextBorderCol() const { return text_border_col_; }
    const sf::Color& getTextHighlightCol() const { return text_highlight_col_; }
    float getBorderSize() const { return border_size_; }
    const sf::Font& getFont() const { return *font_; }

private:
    sf::Color body_col_;
    sf::Color body_highlight_col_;
    sf::Color border_col_;
    sf::Color border_highlight_col_;
    sf::Color text_col_;
    sf::Color text_border_col_;
    sf::Color text_highlight_col_;
    const sf::Font* font_;
    float border_size_;
};

#endif // !GUISTYLE_H_

