#include "../include/no_gravity_game_ws/enemy.hpp"

Enemy::Enemy(sf::Vector2f size, float restitution = 0.f)
{
    this->size_ = size;
    this->is_moving_ = true;
    this->material_.restitution = restitution;
    this->body_ = sf::RectangleShape(size_);
    this->body_.setFillColor(Colors().ENEMY);
}

Enemy::~Enemy() = default;

void Enemy::update(sf::Time dt)
{
    sf::Vector2f direction = sf::Vector2f(target_position_->x - this->getBody().getPosition().x, target_position_->y - this->getBody().getPosition().y);
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    try
    {
        direction *= 1.f/magnitude;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    this->position_.x += direction.x * this->SPEED * dt.asSeconds();
    this->position_.y += direction.y * this->SPEED * dt.asSeconds();
}

void Enemy::move()
{
    this->body_.setOrigin(this->size_.x/2, this->size_.y/2);
    this->body_.move(this->position_);
}

void Enemy::reset()
{
    this->acceleration_ = sf::Vector2f(0, 0);
    this->velocity_ = sf::Vector2f(0, 0);
    this->position_ = sf::Vector2f(0, 0);
    this->orientation_ = 0;
    this->angular_velocity_ = 0;
    this->angular_acceleration_ = 0;
    this->setRandomPosition();
}

// getters
float Enemy::getSpeed() const { return this->SPEED; }

// setters
void Enemy::setSpeed(float speed) { this->SPEED = speed; }
void Enemy::setTarget(const sf::Vector2f* target) { this->target_position_ = target; }