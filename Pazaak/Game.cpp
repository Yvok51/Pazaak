
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <cereal/archives/xml.hpp>
#include "GuiStyle.h"
#include "GameConst.h"
#include "GameStateMenu.h"
#include "Game.h"


static bool fileExists(const std::string& filename) {
    std::filesystem::directory_entry entry{ std::filesystem::path{ filename } };
    return entry.exists() && entry.is_regular_file();
}

static void createFile(const std::string& filename) {
    std::filesystem::path path{ filename };
    std::filesystem::create_directories(path.parent_path());
    std::fstream fs;
    fs.open(path, std::ios::out);
    fs.close();
}

Game::Game(unsigned int width, unsigned int height, unsigned int style_flags) : window_(sf::VideoMode(width, height), "Pazaak", style_flags)
{
    load();
    window_.setFramerateLimit(60);
    setBackground();

    pushState(std::make_unique<GameStateMenu>(GameStateMenu(this, &window_)));
}

/* Main game loop */
void Game::run() {
    sf::Clock clock;

    // Game loop
    while (running())
    {
        sf::Time elapsed = clock.restart();
        auto dt = elapsed.asSeconds();

        // update
        peekState()->update(dt);

        // render
        window_.clear();
        peekState()->draw(dt);
        window_.display();

    }
}

/* Get the full card collection */
CardCollection Game::getFullCardCollection() const
{
    CardCollection collection{ GameConst::TotalNumberOfCards };

    collection.add(std::make_unique<HandCard>(1));
    collection.add(std::make_unique<HandCard>(2));
    collection.add(std::make_unique<HandCard>(3));
    collection.add(std::make_unique<HandCard>(4));
    collection.add(std::make_unique<HandCard>(5));
    collection.add(std::make_unique<HandCard>(6));
    
    collection.add(std::make_unique<HandCard>(-1));
    collection.add(std::make_unique<HandCard>(-2));
    collection.add(std::make_unique<HandCard>(-3));
    collection.add(std::make_unique<HandCard>(-4));
    collection.add(std::make_unique<HandCard>(-5));
    collection.add(std::make_unique<HandCard>(-6));

    collection.add(std::make_unique<FlipableCard>(1));
    collection.add(std::make_unique<FlipableCard>(2));
    collection.add(std::make_unique<FlipableCard>(3));
    collection.add(std::make_unique<FlipableCard>(4));
    collection.add(std::make_unique<FlipableCard>(5));
    collection.add(std::make_unique<FlipableCard>(6));

    collection.add(std::make_unique<TwoAndFourCard>());
    collection.add(std::make_unique<ThreeAndSixCard>());
    collection.add(std::make_unique<DoubleCard>());
    collection.add(std::make_unique<TiebrakerCard>());

    return collection;
}

/* Change the current game state
* Does not delete the current state
*/
void Game::pushState(std::unique_ptr<GameState> state)
{
    state_handler.pushState(std::move(state));
}

/* 
* Delete the current state
*/
void Game::popState()
{
    state_handler.popState();
}

/* Change the current game state
* Deletes the previous state
*/
void Game::changeState(std::unique_ptr<GameState> state)
{
    state_handler.changeState(std::move(state));
}

/* Get current game state 
* throws std::logic_error if no state exists
*/
std::unique_ptr<GameState>& Game::peekState()
{
    return state_handler.peekState();
}

/* Load campaign from the save 
* Throws std::runtime_error if the save is unable to be opened
*/
void Game::loadCampaign()
{
    if (fileExists(GameConst::SaveLocation)) {
        std::ifstream is;
        is.open(GameConst::SaveLocation);
        if (is.good()) {
            cereal::XMLInputArchive archive(is);

            archive(CEREAL_NVP(campaign_));
            is.close();
        }
        else {
            is.close();
            throw std::runtime_error{ "Campaign save file was not able to be opened" };
        }
    }
    else {
        throw std::runtime_error{ "Campaign save file does not exist" };
    }
}

/* Save the campaign to the save file */
void Game::saveCampaign()
{
    if (!fileExists(GameConst::SaveLocation)) {
        createFile(GameConst::SaveLocation);
    }

    std::ofstream os;
    os.open(GameConst::SaveLocation);
    if (os.good()) {
        cereal::XMLOutputArchive archive(os);

        archive(CEREAL_NVP(campaign_));
    }
    else {
        os.close();
        throw std::runtime_error{ "Campaign save file was not able to be opened" };
    }
    os.close();
}

void Game::load()
{
    // order is important: fonts <- styles
    loadTextures();
    loadFonts();
    loadStyles();
}

/* Load all needed textures at the beginning of the game
* they stay in graphics memory for the entire duration of the game,
* which shouldn't be a problem since they don't take up much memory
* 
* throws std::runtime error on unsuccessfull load
*/
void Game::loadTextures()
{
    tex_mgr_.loadTexture("Background", GameConst::TextureBaseDir + "/background.png");
    tex_mgr_.loadTexture("BlueCard", GameConst::TextureBaseDir + "/BlueHandCard.png");
    tex_mgr_.loadTexture("GreenCard", GameConst::TextureBaseDir + "/DealerCard.png");
    tex_mgr_.loadTexture("RedCard", GameConst::TextureBaseDir + "/RedHandCard.png");
    tex_mgr_.loadTexture("BlueAndRedCard", GameConst::TextureBaseDir + "/RedAndBlueHandCard.png");
    tex_mgr_.loadTexture("YellowCard", GameConst::TextureBaseDir + "/YellowHandCard.png");
    tex_mgr_.loadTexture("UnknownCard", GameConst::TextureBaseDir + "/UnknownCard.png");
    tex_mgr_.loadTexture("EmptyCard", GameConst::TextureBaseDir + "/EmptyCardSlot.png");

}

/* Load all needed fonts into memory 
* throws std::runtime error on unsuccessfull load
*/
void Game::loadFonts()
{
    font_mgr_.loadFont("Prototype", GameConst::FontBaseDir + "/Prototype.ttf");
}

/* Create all the needed styles */
void Game::loadStyles()
{
    stylesheets_.emplace(GameConst::ButtonStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color(211, 211, 211), sf::Color::Black, sf::Color::Black, sf::Color::White, // normal (body, border, text, border_text)
        sf::Color(211, 211, 211), sf::Color::Black, sf::Color::Black)); // highlight
    
    stylesheets_.emplace(GameConst::CardStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White, sf::Color::White,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White));
    
    stylesheets_.emplace(GameConst::TransparentRectStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White, sf::Color::White,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White));
    
    stylesheets_.emplace(GameConst::TextStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::Transparent, sf::Color::Black, sf::Color::Black, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White));
    
    stylesheets_.emplace(GameConst::TurnBulbStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::Red, sf::Color::Black, sf::Color::White, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White));
    
    stylesheets_.emplace(GameConst::SetBulbStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::Yellow, sf::Color::Black, sf::Color::White, sf::Color::Black,
        sf::Color::Transparent, sf::Color::Black, sf::Color::White));
    
    stylesheets_.emplace(GameConst::PromptStyle, GuiStyle(&font_mgr_.getFont("Prototype"), 2.f,
        sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::Black,
        sf::Color::White, sf::Color::Black, sf::Color::White));
}

void Game::setBackground()
{
    background_.setTexture(tex_mgr_.getTexture("Background"));
}
