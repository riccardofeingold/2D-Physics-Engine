#include "../include/no_gravity_game_ws/window.hpp"

Window::Window()
{
    Window::setup("Window", sf::Vector2u(640, 480), 60);
}

Window::Window(const std::string& title, const sf::Vector2u& size, const unsigned int& frame_rate)
{
    Window::setup(title, size, frame_rate);
}

Window::~Window()
{
    Window::destroy();
}

void Window::setup(const std::string& title, const sf::Vector2u& size, const unsigned int& frame_rate)
{
    this->window_size_ = size;
    this->window_title_ = title;
    this->frame_rate_ = frame_rate;
    this->is_done_ = false;
    this->is_fullscreen_ = false;
    
    // create window
    Window::create();
}

void Window::create()
{
    uint32_t style = this->is_fullscreen_ ? sf::Style::Fullscreen : sf::Style::Default;
    sf::VideoMode video_mode(this->window_size_.x, this->window_size_.y, 32);
    this->window_.create(video_mode, this->window_title_, style);
    this->window_.setFramerateLimit(this->frame_rate_);
}

void Window::destroy()
{
    this->window_.close();
}

void Window::update()
{
    sf::Event event;

    while (this->window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            this->is_done_ = true;
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
        {
            Window::toggleFullscreen();
        }
    }
}

void Window::toggleFullscreen()
{
    this->is_fullscreen_ = !this->is_fullscreen_;
    Window::destroy();
    Window::create();
}

void Window::beginDraw()
{
    this->window_.clear();
}

void Window::endDraw()
{
    this->window_.display();
}

void Window::draw(const sf::Drawable& drawable)
{
    this->window_.draw(drawable);
}

bool Window::isDone() const
{
    return this->is_done_;
}

bool Window::isFullscreen() const
{
    return this->is_fullscreen_;
}

sf::Vector2u Window::getWindowSize() const
{
    return this->window_size_;
}