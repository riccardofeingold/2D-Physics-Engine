#include <iostream>
#include <SFML/Graphics.hpp>
#include "include/no_gravity_game_ws/player.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPEED 200
#define GRAVITY 9.81

int main()
{
    // set window size
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "My first game using C++!");
    window.setFramerateLimit(60);
    // clock
    sf::Clock clock;

    // player
    Player player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 1.f);

    // check if window has been closed by user
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // getting delta time
        sf::Time dt = clock.restart();

        // gravity
        player.velocity.y += GRAVITY * dt.asSeconds();
        player.body.move(0.f, player.velocity.y * dt.asSeconds());

        // collision
        // with bottom
        if (player.body.getPosition().y + 2*player.size >= SCREEN_HEIGHT)
            player.velocity.y = -player.velocity.y * player.restitution;

        // steering
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.body.move(0.f, -SPEED * dt.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.body.move(0.f, SPEED * dt.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.body.move(-SPEED * dt.asSeconds(), 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.body.move(SPEED * dt.asSeconds(), 0.f);

        // draw something
        window.clear();
        window.draw(player.body);
        window.display();

    }


    return 0;
}