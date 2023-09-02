#include "../include/no_gravity_game_ws/game.hpp"
#define debugging true

Game::Game() : 
    window_("2D Drone Simulator", sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT), FRAME_RATE) 
    // player_(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, sf::Vector2f(40, 40), 0.f),
    // goal_(sf::Vector2f(20, 20), 0),
    // enemy_(sf::Vector2f(40, 40), 0.f),
    // tester_(sf::Vector2f(40, 40), 0.f)
{
    this->world_.window_ = &window_;
    
    this->start();
}

Game::~Game()
{
    for (int i = 0; i < this->world_.getBodyCount(); ++i)
    {
        Rigidbody* body;
        if (this->world_.getBody(i, body))
        {
            delete body;
        }
    }
    // for (auto r : rigidbodies)
    // {
    //     if (r != nullptr)
    //         delete r;
    // }
}

void Game::start()
{
    // Camera settings
    this->view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->view.zoom(0.08f);
    this->view.setCenter(this->view.getSize().x/2, this->view.getSize().y/2);
    const float padding = this->view.getSize().x * 0.05f;
    this->world_.window_->getWindow().setView(view);

    /********TESTING*********/
    for (int i = 0; i < 10; ++i)
    {
        int shape_random = (float)std::rand()/RAND_MAX > 0.5 ? 1 : 0;
        Rigidbody* body = nullptr;

        sf::View current_view = this->world_.window_->getWindow().getView();

        Vector2f position((float)std::rand()/RAND_MAX * (current_view.getSize().x - padding), (float)std::rand()/RAND_MAX * (current_view.getSize().y - padding));

        std::string e;
        if (shape_random == ShapeType::Circle)
        {
            if (!Rigidbody::createCircleBody(1.f, position, 2.f, false, 1.f, body, e))
                std::cout << e << std::endl;
            else
                this->world_.addObject(std::to_string(i), body);
        } else if (shape_random == ShapeType::Box)
        {
            if (!Rigidbody::createBoxBody(1.77f, 1.77f, position, 2.f, false, 1.f, body, e))
                std::cout << e << std::endl;
            else
            {
                this->world_.addObject(std::to_string(i), body);
            }
        } else
        {
            std::cout << "unknown type" << std::endl;
        }
    }

    // add walls
    for (int i = 0; i < 10; ++i)
    {
        Rigidbody* body = nullptr;
        int shape_random = (float)std::rand()/RAND_MAX > 0.5 ? 1 : 0;
        Vector2f position = Vector2f((float)std::rand()/RAND_MAX * this->view.getSize().x, (float)std::rand()/RAND_MAX * this->view.getSize().y);
        std::string e = "Error in creating static box!";
        if (shape_random == ShapeType::Circle)
        {
            if (!Rigidbody::createCircleBody(1.f, position, 2.f, true, (float)std::rand()/RAND_MAX, body, e))
                std::cout << e << std::endl;
            else
            {
                body->color = sf::Color::Black;
                this->world_.addObject(std::to_string(i), body);
            }
        } else if (shape_random == ShapeType::Box)
        {
            if (!Rigidbody::createBoxBody(1.77f, 1.77f, position, 2.f, true, (float)std::rand()/RAND_MAX, body, e))
                std::cout << e << std::endl;
            else
            {
                body->color = sf::Color::Black;
                this->world_.addObject(std::to_string(i), body);
            }
        } else
        {
            std::cout << "unknown type" << std::endl;
        }
    }

    /********TESTING*********/
#if !debugging
    // goals
    this->goal_.setWindow(window_);
    this->goal_.setRandomPosition();

    // enemy
    this->enemy_.setSpeed(5);
    this->enemy_.setWindow(window_);
    this->enemy_.setRandomPosition();
    this->enemy_.setTarget(&(this->player_.getBody().getPosition()));

    // tester
    this->tester_.setSpeed(0);
    this->tester_.setWindow(window_);

    // set properties of player
    this->player_.setWindow(window_);
    this->player_.applyAirDrag(DRAG_COEFFICIENT, 1, AIR_DENSITY);
    this->player_.setMass(1);

    // add objects to the world
    this->world_.addObject("player", &this->player_);
    this->world_.addObject("goal", &this->goal_);
    this->world_.addObject("enemy", &this->enemy_);
    this->world_.addObject("tester", &this->tester_);

    // walls
    for (int i = 0; i < NUM_WALLS; ++i)
    {
        float random_width = std::max((float)std::rand()/RAND_MAX*MAX_WALL_SIZE, 20.f);
        float random_height = std::max((float)std::rand()/RAND_MAX*MAX_WALL_SIZE, 20.f);
        Wall wall = Wall(sf::Vector2f((float)std::rand()/RAND_MAX*(i+1)*SCREEN_WIDTH/NUM_WALLS, (float)std::rand()/RAND_MAX*(i+1)*SCREEN_HEIGHT/NUM_WALLS), sf::Vector2f(random_width, random_height), (float)std::rand()/RAND_MAX*180);
        this->walls_.push_back(wall);
    }

    int counter = 0;
    for (auto it = this->walls_.begin(); it != this->walls_.end(); ++it)
    {
        std::string name = "wall";
        name += std::to_string(counter);
        this->world_.addObject(name, &*it);
        ++counter;
        while (this->world_.checkCollisionBetween("goal", name, this->dt_))
        {
            this->goal_.setRandomPosition();
        }
        while (this->world_.checkCollisionBetween("player", name, this->dt_))
        {
            this->world_.getObject("name").getBody().move(sf::Vector2f((float)std::rand()/RAND_MAX*20, (float)std::rand()/RAND_MAX*20));
        }
    }
#endif
}

void Game::handleInput()
{
    // keyboard steering
    Vector2f dv(0.f, 0.f);
    float delta_rotation = 0.f;
    float angular_speed = M_PI/4;
    float force_magnitude = 60.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        --dv.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        ++dv.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        --dv.x;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        ++dv.x;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        ++delta_rotation;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        --delta_rotation;

    if (dv.x != 0 || dv.y != 0)
    {
        Vector2f force_direction = Math2D::normalize(dv);
        Vector2f force = force_direction * force_magnitude;
        Rigidbody* body;
        assert(this->world_.getBody(0, body));
        body->applyForce(force);
    }

    if (delta_rotation != 0)
    {
        int sign = delta_rotation < 0 ? -1 : 1;
        float rotation = sign * angular_speed * this->dt_.asSeconds();
        Rigidbody* body;
        assert(this->world_.getBody(0, body));
        body->rotate(rotation);
    }
    #if !debugging
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
    
    this->tester_.getBody().setPosition(sf::Mouse::getPosition().x * SCREEN_WIDTH / 5118.f, (sf::Mouse::getPosition().y - 2880.f) * SCREEN_HEIGHT / (5758.f - 2880.f));
    #endif
}

void Game::update()
{
    this->dt_ = this->clock_.restart();
    this->window_.update();
    this->world_.update(this->dt_);
    this->wrapScreen();
}

void Game::render()
{
    this->window_.beginDraw();

    for (int i = 0; i < this->world_.getBodyCount(); ++i)
    {
        Rigidbody* b;

        assert(this->world_.getBody(i, b));

        sf::Vector2f pos = Vector2Converter::toSFVector2f(b->getPosition());
        if (b->shape_type == ShapeType::Circle)
        {
            sf::CircleShape circle(b->radius);
            circle.setFillColor(b->color);
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(0.1f);
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(pos);
            this->window_.draw(circle);
        } else if (b->shape_type == ShapeType::Box)
        {
            Vector2Converter::toSFVector2fList(b->getTransformedVertices(), this->vertex_buffer_);

            sf::ConvexShape box(4);
            for (int i = 0; i < box.getPointCount(); ++i)
            {
                box.setPoint(i, this->vertex_buffer_[i]);
            }

            box.setFillColor(b->color);
            box.setOutlineThickness(0.1f);
            box.setOrigin(pos);
            box.setPosition(pos);
            this->window_.draw(box);
        }
    }
    /********TESTING**********/
    #if !debugging
    for (auto r : this->world_.getObject("player").rays)
    {
        r.castRay(this->world_);
        r.draw();
        this->window_.draw(r.getLineShape());
    }

    for (std::string name : this->world_.list_of_object_names_)
    {
        this->window_.draw(this->world_.getObject(name).getBody());
    }

    if (this->world_.game_over)
    {
        std::cout << "GAME OVER! 😂" << " Your score: " << this->player_.getScore() << std::endl;
        this->player_.setScoreToZero();
    }
    #endif

    this->window_.endDraw();
    
    if (this->world_.game_over)
    {
        // sf::sleep(sf::Time(sf::seconds(3)));
        this->world_.game_over = false;
    }
}

Window* Game::getWindow()
{
    return &(this->window_);
}

void Game::wrapScreen()
{
    for (int i = 0; i < this->world_.getBodyCount(); ++i)
    {
        Rigidbody* body;
        assert(this->world_.getBody(i, body));

        if (body->getPosition().x > this->view.getSize().x)
        {
            body->moveTo(Vector2f(0, body->getPosition().y));
        }

        if (body->getPosition().x < 0)
        {
            body->moveTo(Vector2f(this->view.getSize().x, body->getPosition().y));
        }

        if (body->getPosition().y > this->view.getSize().y)
        {
            body->moveTo(Vector2f(body->getPosition().x, 0));
        }

        if (body->getPosition().y < 0)
        {
            body->moveTo(Vector2f(body->getPosition().x, this->view.getSize().y));
        }
    }
}