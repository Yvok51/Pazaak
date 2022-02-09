// Pazaak.cpp : Defines the entry point for the application.
//

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.h"

enum class ExitCode
{
    OK = 0,
    Exception = 1,
    UnknownError = 2
};

int main()
{
    ExitCode exit_code = ExitCode::OK;
    try {
        Game game(static_cast<unsigned int>(GameConst::ScreenSize.x), static_cast<unsigned int>(GameConst::ScreenSize.y), sf::Style::Titlebar | sf::Style::Close);

        game.run();
    }
    catch (const std::exception& e) {
        exit_code = ExitCode::Exception;
        std::cerr << e.what() << std::endl;
        std::cerr << "Exiting with code " + std::to_string(static_cast<int>(exit_code)) << std::endl;
    }
    catch (...) {
        exit_code = ExitCode::UnknownError;
        std::cerr << "Unknown exception has occured" << std::endl;
        std::cerr << "Exiting with code " + std::to_string(static_cast<int>(exit_code)) << std::endl;
    }

    return static_cast<int>(exit_code);
}