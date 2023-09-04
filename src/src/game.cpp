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
        Rigidbody* body = nullptr;
        if (this->world_.getBody(i, body))
        {
            delete body;
        }
    }
}

void Game::start()
{
    // start second stop watch
    this->watch2_start_ = std::chrono::high_resolution_clock::now();

    // Camera settings
    this->view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->view.zoom(ZOOM);
    this->view.setCenter(this->view.getSize().x/2, this->view.getSize().y/2);
    const float padding = this->view.getSize().x * 0.05f;
    this->world_.window_->getWindow().setView(view);

    std::string e = "Error!";
    for (int i = 0; i < 2; ++i)
    {
        Rigidbody* body = nullptr;
        if (i == 0)
        {
            if (!Rigidbody::createBoxBody("player", 1.77f, 1.77f, Vector2f(this->view.getSize().x / 2, this->view.getSize().y / 2), 2.f, false, 1.f, body, e, true))
                std::cout << e << std::endl;
            else
            {
                body->color = Colors().PLAYER;
                this->world_.addObject(body->name, body);
            }
            continue;
        } else if (i == 1)
        {
            if (!Rigidbody::createBoxBody("ground", this->view.getSize().x - padding * 2, 3.f, Vector2f(this->view.getSize().x/2, this->view.getSize().y - 1.5f - padding), 2.f, true, 0.f, body, e))
                std::cout << e << std::endl;
            else 
            {
                body->color = Colors().WALL;
                this->world_.addObject(body->name, body);
            }
            continue;
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
    float force_magnitude = 100.f;

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
        Rigidbody* body = nullptr;
        assert(this->world_.getBody("player", body));
        body->applyForce(force);
    }

    if (delta_rotation != 0)
    {
        int sign = delta_rotation < 0 ? -1 : 1;
        float rotation = sign * angular_speed * this->dt_.asSeconds();
        Rigidbody* body = nullptr;
        assert(this->world_.getBody("player", body));
        body->rotate(rotation);
    }

    // add box
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->window_.mouse_button_pressed)
    {
        float width = std::max((float)std::rand()/RAND_MAX * 3, 1.f);
        float height = std::max((float)std::rand()/RAND_MAX * 3, 1.f);
        Rigidbody* body = nullptr;
        std::string e = "ERROR";
        Vector2f position = Vector2f((float)sf::Mouse::getPosition(this->window_.getWindow()).x * ZOOM, (float)sf::Mouse::getPosition(this->window_.getWindow()).y * ZOOM);
        
        if (!Rigidbody::createBoxBody(std::to_string(this->world_.getBodyCount()), width, height, position, 2.f, false, 1.f, body, e, true))
            std::cout << e << std::endl;
        else
            this->world_.addObject(body->name, body);

        this->window_.mouse_button_pressed = true;
    }

    // add circle
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !this->window_.mouse_button_pressed)
    {
        float radius = std::max((float)std::rand()/RAND_MAX * 3, 1.f);
        Rigidbody* body = nullptr;
        std::string e = "ERROR";
        Vector2f position = Vector2f((float)sf::Mouse::getPosition(this->window_.getWindow()).x * ZOOM, (float)sf::Mouse::getPosition(this->window_.getWindow()).y * ZOOM);

        if (!Rigidbody::createCircleBody(std::to_string(this->world_.getBodyCount()), radius, position, 2.f, false, 1.f, body, e, true))
            std::cout << e << std::endl;
        else
            this->world_.addObject(body->name, body);

        this->window_.mouse_button_pressed = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        std::cout << "Body count: " << this->world_.getBodyCount() << std::endl;
        std::cout << "Time for physics step: " << this->duration_.count() << std::endl;
        std::cout << "Number of contact points: " << this->world_.contact_points.size() << std::endl;
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
    // second stopwatch
    this->watch2_stop_ = std::chrono::high_resolution_clock::now();
    this->duration2_ = std::chrono::duration_cast<std::chrono::microseconds>(this->watch2_stop_ - this->watch2_start_);
    if (this->duration2_.count() > 1000000)
    {
        this->time_step_string_ = std::to_string((float)this->total_world_time_step_ / this->total_samples_);
        this->body_count_string_ = std::to_string((float)this->total_body_count_ / this->total_samples_);
        this->total_world_time_step_ = 0;
        this->total_body_count_ = 0;
        this->total_samples_ = 0;
        this->watch2_start_ = std::chrono::high_resolution_clock::now();
    }

    this->dt_ = this->clock_.restart();
    this->window_.update();

    this->watch_start_ = std::chrono::high_resolution_clock::now();
    this->world_.update(this->dt_, 20);
    this->watch_stop_ = std::chrono::high_resolution_clock::now();
    this->duration_ = std::chrono::duration_cast<std::chrono::microseconds>(this->watch_stop_ - this->watch_start_);

    this->total_world_time_step_ += duration_.count();
    this->total_body_count_ += this->world_.getBodyCount();
    ++this->total_samples_;

    for (int i = 0; i < this->world_.getBodyCount(); ++i)
    {
        Rigidbody* body = nullptr;
        if (!this->world_.getBody(i, body))
        {
            std::cout << "ERROR" << std::endl;
        }

        AABB box = body->getAABB();

        if (box.min.y > this->view.getSize().y)
        {
            this->world_.removeObject(body->name);
        }
    }
}

void Game::render()
{
    this->window_.beginDraw();

    for (int i = 0; i < this->world_.getBodyCount(); ++i)
    {
        Rigidbody* b = nullptr;

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

    // draw contact points
    for (int i = 0; i < this->world_.contact_points.size(); ++i)
    {
        float radius = 0.3f;
        sf::CircleShape point(radius);
        point.setFillColor(sf::Color::Red);
        point.setOrigin(radius, radius);
        point.setPosition(this->world_.contact_points[i].x, this->world_.contact_points[i].y);
        this->window_.draw(point);
    }

    // draw text
    sf::Font font;
    if (!font.loadFromFile("/Users/riccardofeingold/opt/GameCreations/no_gravity_game_ws/fonts/Roboto-Light.ttf"))
        std::cout << "Couldn't find font!" << std::endl;

    sf::Text step_time_text;
    step_time_text.setString("Step Time: " + this->time_step_string_ + " microseconds\n" + "Body Count: " + this->body_count_string_);
    step_time_text.setStyle(sf::Text::Regular);
    step_time_text.setFillColor(sf::Color::White);
    step_time_text.setFont(font);
    step_time_text.setCharacterSize(20);
    step_time_text.setScale(ZOOM, ZOOM);
    step_time_text.setPosition(0, 0);
    this->window_.draw(step_time_text);

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
        Rigidbody* body = nullptr;
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