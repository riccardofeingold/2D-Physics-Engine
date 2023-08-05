#include <iostream>
#include <SFML/Graphics.hpp>
#include "include/player.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPEED 5000

int main()
{
    // set window size
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "My first game using C++!");

    // delta time
    sf::Clock delta_time_clock;
    sf::Time dt;

    // player
    Player player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 50);
    sf::Vector2f position = player.shape_.getPosition();

    // check if window has been closed by user
    while (window.isOpen())
    {
        dt = delta_time_clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                position.x += SPEED*dt.asSeconds();
        }

        // movement
        player.shape_.setPosition(position.x, player.shape_.getPosition().y);

        // draw something
        window.clear();
        window.draw(player.shape_);
        window.display();

    }


    return 0;
}