#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Window
{
    public:
    // constructors
    Window();
    Window(const std::string& title, const sf::Vector2u& size, const unsigned int& frame_rate);
    
    // deconstructor
    ~Window();

    // methods
    void beginDraw(); // Clear screen
    void update(); // prepare for display
    void endDraw(); // Display

    void toggleFullscreen();
    void draw(const sf::Drawable& drawable);

    // getters and setters
    bool isDone() const;
    bool isFullscreen() const;
    sf::Vector2u getWindowSize() const;

    private:
    // methods
    void setup(const std::string& title, const sf::Vector2u& size, const unsigned int& frame_rate);
    void destroy();
    void create();
    
    // private variables
    unsigned int frame_rate_;
    sf::RenderWindow window_;
    sf::Vector2u window_size_;
    std::string window_title_;
    bool is_done_;
    bool is_fullscreen_;
};