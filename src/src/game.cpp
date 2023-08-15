#include "../include/no_gravity_game_ws/game.hpp"

Game::Game() : 
    window_("2D Drone Simulator", sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT), FRAME_RATE), 
    player_(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 1.f),
    test_(0, 0, 10, 1)
{
    this->player_.applyAirDrag(DRAG_COEFFICIENT, 1, AIR_DENSITY);
    this->test_.applyAirDrag(DRAG_COEFFICIENT, 1, AIR_DENSITY);
    this->start();
}

Game::~Game() = default;

void Game::start()
{
    // set properties of player
    this->player_.setMass(1);
    this->player_.setMomentOfInertia(1);
    world_.addObject(&this->player_);

    world_.addObject(&this->test_);
}

void Game::handleInput()
{
    if (sf::Joystick::isConnected(0))
    {
        int throttle = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        int roll = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

        this->player_.apply_force(sf::Vector2f(0.f, MAX_THRUST_FORCE * throttle));
        this->player_.apply_force(sf::Vector2f(MAX_THRUST_FORCE * roll, 0.f));
    } else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            this->player_.move(sf::Vector2f(1.f, -SPEED * this->dt_.asSeconds()));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            this->player_.move(sf::Vector2f(1.f, SPEED * this->dt_.asSeconds()));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            this->player_.move(sf::Vector2f(-SPEED * this->dt_.asSeconds(), 0.f));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            this->player_.move(sf::Vector2f(SPEED * this->dt_.asSeconds(), 0.f));
    }
}

void Game::update()
{
    this->dt_ = this->clock_.restart();
    this->window_.update();
    this->world_.update(this->dt_);
}

void Game::render()
{
    this->window_.beginDraw();
    for (int i = 0; i < this->world_.getNumberOfObjects(); ++i)
    {
        this->window_.draw(this->world_.getObject(i).getBody());
    }
    this->window_.endDraw();
}

Window* Game::getWindow()
{
    return &(this->window_);
}