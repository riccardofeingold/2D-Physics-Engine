#include "../include/no_gravity_game_ws/game.hpp"

Game::Game() : 
    window_("2D Drone Simulator", sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT), FRAME_RATE), 
    player_(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 1.f), 
    playerPhysics_(GRAVITY, 1, 1) 
{
}

Game::~Game() = default;

void Game::handleInput()
{
    if (sf::Joystick::isConnected(0))
    {
        int throttle = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        int roll = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

        // player_.setVelocity(sf::Vector2f(player_.getVelocity().x, player_.getVelocity().y + SPEED_JOYSTICK * throttle));
        // player_.setVelocity(sf::Vector2f(player_.getVelocity().x + SPEED_JOYSTICK * roll, player_.getVelocity().y));

        this->playerPhysics_.apply_force(sf::Vector2f(0.f, MAX_THRUST_FORCE * throttle));
        this->playerPhysics_.apply_force(sf::Vector2f(MAX_THRUST_FORCE * roll, 0.f));
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
    this->playerPhysics_.update(this->dt_);
    this->player_.move(this->playerPhysics_.get_position());
}

void Game::render()
{
    this->window_.beginDraw();
    this->window_.draw(this->player_.body);
    this->window_.endDraw();
}

Window* Game::getWindow()
{
    return &(this->window_);
}