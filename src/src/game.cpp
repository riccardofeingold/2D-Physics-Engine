#include "../include/no_gravity_game_ws/game.hpp"

Game::Game() : 
    window_("2D Drone Simulator", sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT), FRAME_RATE), 
    player_(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, sf::Vector2f(40, 40), 1.f),
    goal_(sf::Vector2f(20, 20), 0),
    enemy_(sf::Vector2f(40, 40), 0.f)
{
    this->world_.window_ = &window_;
    
    this->start();
}

Game::~Game() = default;

void Game::start()
{
    this->goal_.setWindow(window_);
    this->goal_.setRandomPosition();

    this->enemy_.setSpeed(5);
    this->enemy_.setWindow(window_);
    this->enemy_.setRandomPosition();
    this->enemy_.setTarget(&(this->player_.getBody().getPosition()));
    
    // set properties of player
    this->player_.setWindow(window_);
    this->player_.applyAirDrag(DRAG_COEFFICIENT, 1, AIR_DENSITY);
    this->player_.setMass(1);
    this->world_.addObject("player", &this->player_);
    this->world_.addObject("goal", &this->goal_);
    this->world_.addObject("enemy", &this->enemy_);
}

void Game::handleInput()
{
    if (sf::Joystick::isConnected(0))
    {
        int throttle = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        int roll = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

        this->player_.applyForce(sf::Vector2f(0.f, MAX_THRUST_FORCE * throttle));
        this->player_.applyForce(sf::Vector2f(MAX_THRUST_FORCE * roll, 0.f));

        // restart
        if (sf::Joystick::isButtonPressed(0, 0))
        {
            this->world_.reset();
        }
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
    for (std::string name : this->world_.list_of_object_names_)
    {
        this->window_.draw(this->world_.getObject(name).getBody());
    }

    if (this->world_.game_over)
    {
        std::cout << "GAME OVER! 😂" << " Your score: " << this->player_.getScore() << std::endl;
        this->player_.setScoreToZero();
    }
    this->window_.endDraw();
    
    if (this->world_.game_over)
    {
        sf::sleep(sf::Time(sf::seconds(3)));
        this->world_.game_over = false;
    }
}

Window* Game::getWindow()
{
    return &(this->window_);
}