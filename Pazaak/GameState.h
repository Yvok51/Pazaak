
#include <SFML/Graphics.hpp>

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

class Game;

// Class representing current state of the program
class GameState {
public:
	virtual ~GameState() noexcept = default;
	virtual void update(const double dt) = 0;
	virtual void draw(const double dt) = 0;
protected:
	// Observer pointers
	Game* game_{ nullptr };
	sf::RenderWindow* window_{ nullptr };
};

#endif // !GAMESTATE_H_

