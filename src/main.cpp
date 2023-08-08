#include <iostream>
#include <SFML/Graphics.hpp>
#include "include/no_gravity_game_ws/player.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPEED 200
#define SPEED_JOYSTICK 0.02 // maximum value of the joystick is 100
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

        // collision
        // with top
        if (player.body.getPosition().y <= 0)
            player.velocity.y = player.velocity.y * player.restitution;
        // with bottom
        if (player.body.getPosition().y + 2*player.size >= SCREEN_HEIGHT)
            player.velocity.y = -player.velocity.y * player.restitution;

        // steering
        if (sf::Joystick::isConnected(0))
        {
            // std::cout << "PovX: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) << std::endl;
            // std::cout << "PovY: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) << std::endl;
            // std::cout << "R: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) << std::endl;
            // std::cout << "U: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) << std::endl;
            // std::cout << "V: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V) << std::endl;
            // std::cout << "X: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) << std::endl;
            // std::cout << "Y: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) << std::endl;
            // std::cout << "Z: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) << std::endl;
            int throttle = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
            int roll = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);

            player.velocity.y += SPEED_JOYSTICK * throttle;
            player.velocity.x += SPEED_JOYSTICK * roll;

        } else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                player.body.move(0.f, -SPEED * dt.asSeconds());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                player.body.move(0.f, SPEED * dt.asSeconds());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                player.body.move(-SPEED * dt.asSeconds(), 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                player.body.move(SPEED * dt.asSeconds(), 0.f);
        }

        // send commands for movement
        player.body.move(player.velocity.x * dt.asSeconds(), player.velocity.y * dt.asSeconds());

        // draw something
        window.clear();
        window.draw(player.body);
        window.display();
    }

    return 0;
}