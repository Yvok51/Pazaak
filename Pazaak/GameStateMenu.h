
#include <string>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Game.h"
#include "GuiMenu.h"
#include "Prompt.h"

#ifndef GAMESTATEMENU_H_
#define GAMESTATEMENU_H_

class GameStateMenu final : public GameState {
public:
    GameStateMenu(Game* game, sf::RenderWindow* window);
    virtual void draw(const double dt) override;
    virtual void update(const double dt) override;
private:
    void handleInput();

    void pause() { paused_ = true; }
    void unpause() { paused_ = false; }
    bool paused() { return paused_; }

    // Button actions
    void loadQuickMatch();
    void loadNewCampaign();
    void loadOldCampaign();
    void exit();

    void setupPrompt();

    sf::View view_;

    GuiMenu gui_;

    /* Whether the prompt is shown and the menu is paused */
    bool paused_;

    static inline const sf::Vector2f PromptSize = { 350, 150 };
    Prompt prompt_;
};

#endif // !GAMESTATEMENU_H_

