#include "../include/no_gravity_game_ws/ray.hpp"

Ray::Ray(const sf::Vector2f* start, float angle)
    : start_(start),
      angle_(angle)
{
    float rad = angle_ * M_PI / 180;
    this->direction_ = sf::Vector2f(std::cos(rad), std::sin(rad));
    std::cout << direction_.x << " " << direction_.y << std::endl;
    this->line_ = sf::RectangleShape(sf::Vector2f(0, THICKNESS));
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(*start_);
    this->line_.setFillColor(sf::Color::White);

}

Ray::~Ray() = default;

void Ray::draw()
{
    // std::cout << this->point_of_contact_.x << " " << this->point_of_contact_.y << std::endl;
    sf::Vector2f line_vector = *this->start_ - this->point_of_contact_;
    float magnitude = std::sqrt(line_vector.x * line_vector.x + line_vector.y * line_vector.y);
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(*start_);

    if (magnitude <= MAX_DISTANCE)
        line_.setSize(sf::Vector2f(magnitude, THICKNESS));
    else
        line_.setSize(sf::Vector2f(MAX_DISTANCE, THICKNESS));
        
    line_.rotate(this->angle_);
}

void Ray::castRay(World& w)
{
    const sf::Vector2f& player = w.getObject("player").getBody().getPosition();

    // wikipedia article about line intersection: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    for (auto obj : w.objects_)
    {
        if (obj.first != "player")
        {
            const sf::Vector2f& tl = obj.second->getCornerPosition("tl");
            const sf::Vector2f& bl = obj.second->getCornerPosition("bl");
            const sf::Vector2f& tr = obj.second->getCornerPosition("tr");
            const sf::Vector2f& br = obj.second->getCornerPosition("br");

            // left side
            float den = (tl.x - bl.x) * (player.y - (player.y + direction_.y))
                      - (tl.y - bl.y) * (player.x - (player.x + direction_.x));
            
            if (den != 0)
            {
                float t_num = (tl.x - player.x) * (player.y - (player.y + direction_.y)) - (tl.y - player.y) * (player.x - (player.x + direction_.x));
                float u_num = (tl.x - player.x) * (tl.y - bl.y) - (tl.y - player.y) * (tl.x - bl.x);
                
                float t = t_num / den;
                float u = u_num / den;
                if (t <= 1 && t >= 0 && u >= 0)
                {
                    this->point_of_contact_.x = tl.x + t * (bl.x - tl.x);
                    this->point_of_contact_.y = tl.y + t * (bl.y - tl.y);
                    // std::cout << "x: " << this->point_of_contact_.x << " y: " << this->point_of_contact_.y << std::endl;
                    break;
                }
            }

            // top side
            // float den = (tl.x - bl.x) * (player.y - (player.y + direction_.y))
            //       - (tl.y - bl.y) * (player.x - (player.x + direction_.x));
        
            // if (den != 0)
            // {
            //     float t_num = (tl.x - player.x) * (player.y - (player.y + direction_.y)) - (tl.y - player.y) * (player.x - (player.x + direction_.x));
            //     float u_num = (tl.x - player.x) * (tl.y - bl.y) - (tl.y - player.y) * (tl.x - bl.x);
                
            //     float t = t_num / den;
            //     float u = u_num / den;
            //     if (t <= 1 && t >= 0 && u >= 0)
            //     {
            //         this->point_of_contact_.x = tl.x + t * (bl.x - tl.x);
            //         this->point_of_contact_.y = tl.y + t * (bl.y - tl.y);
            //         found_contact_point = true;
            //     }
            // }

            // right side
            // float den = (tl.x - bl.x) * (player.y - (player.y + direction_.y))
            //       - (tl.y - bl.y) * (player.x - (player.x + direction_.x));
        
            // if (den != 0)
            // {
            //     float t_num = (tl.x - player.x) * (player.y - (player.y + direction_.y)) - (tl.y - player.y) * (player.x - (player.x + direction_.x));
            //     float u_num = (tl.x - player.x) * (tl.y - bl.y) - (tl.y - player.y) * (tl.x - bl.x);
                
            //     float t = t_num / den;
            //     float u = u_num / den;
            //     if (t <= 1 && t >= 0 && u >= 0)
            //     {
            //         this->point_of_contact_.x = tl.x + t * (bl.x - tl.x);
            //         this->point_of_contact_.y = tl.y + t * (bl.y - tl.y);
            //         found_contact_point = true;
            //     }
            // }

            // bottom side
            // float den = (tl.x - bl.x) * (player.y - (player.y + direction_.y))
            //       - (tl.y - bl.y) * (player.x - (player.x + direction_.x));
        
            // if (den != 0)
            // {
            //     float t_num = (tl.x - player.x) * (player.y - (player.y + direction_.y)) - (tl.y - player.y) * (player.x - (player.x + direction_.x));
            //     float u_num = (tl.x - player.x) * (tl.y - bl.y) - (tl.y - player.y) * (tl.x - bl.x);
                
            //     float t = t_num / den;
            //     float u = u_num / den;
            //     if (t <= 1 && t >= 0 && u >= 0)
            //     {
            //         this->point_of_contact_.x = tl.x + t * (bl.x - tl.x);
            //         this->point_of_contact_.y = tl.y + t * (bl.y - tl.y);
            //         found_contact_point = true;
            //     }
            // }

            // if nothing has been found set contact point to start
            this->point_of_contact_ = *this->start_ + sf::Vector2f(200, 200);
        }
    }
}