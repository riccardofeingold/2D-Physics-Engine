#include "../include/no_gravity_game_ws/ray.hpp"

Ray::Ray(const sf::Vector2f* start, float angle)
    : start_(start),
      angle_(angle)
{
    float rad = angle_ * M_PI / 180;
    this->direction_ = sf::Vector2f(std::cos(rad), std::sin(rad));
    this->line_ = sf::RectangleShape(sf::Vector2f(0, THICKNESS));
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(*start_);
    this->line_.setFillColor(sf::Color::White);

}

Ray::~Ray() = default;

// getters
const sf::RectangleShape& Ray::getLineShape() const { return this->line_; }
const float Ray::getDistance() const { return this->distance_; }
const float Ray::getAngle() const { return this->angle_; }

float Ray::calculateMagnitude(sf::Vector2f& end_point)
{
    sf::Vector2f line_vector = *this->start_ - end_point;
    return std::sqrt(line_vector.x * line_vector.x + line_vector.y * line_vector.y);
}

void Ray::draw()
{
    this->distance_ = this->calculateMagnitude(this->point_of_contact_);
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(*start_);

    if (this->distance_ <= MAX_DISTANCE)
        line_.setSize(sf::Vector2f(this->distance_, THICKNESS));
    else
        line_.setSize(sf::Vector2f(MAX_DISTANCE, THICKNESS));
        
    line_.rotate(this->angle_);
}

void Ray::castRay(World& w)
{
    // TODO: minimal distance not working properly
    const sf::Vector2f& player = w.getObject("player").getBody().getPosition();
    // wikipedia article about line intersection: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    for (auto obj : w.objects_)
    {
        if (obj.first != "player")
        {
            std::vector<sf::Vector2f> points;

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

                    points.push_back(this->point_of_contact_);
                }
            }

            // top side
            den = (tl.x - tr.x) * (player.y - (player.y + direction_.y))
                  - (tl.y - tr.y) * (player.x - (player.x + direction_.x));
        
            if (den != 0)
            {
                float t_num = (tl.x - player.x) * (player.y - (player.y + direction_.y)) - (tl.y - player.y) * (player.x - (player.x + direction_.x));
                float u_num = (tl.x - player.x) * (tl.y - tr.y) - (tl.y - player.y) * (tl.x - tr.x);
                
                float t = t_num / den;
                float u = u_num / den;
                if (t <= 1 && t >= 0 && u >= 0)
                {
                    this->point_of_contact_.x = tl.x + t * (tr.x - tl.x);
                    this->point_of_contact_.y = tl.y + t * (tr.y - tl.y);

                    points.push_back(this->point_of_contact_);
                }
            }

            // right side
            den = (tr.x - br.x) * (player.y - (player.y + direction_.y))
                  - (tr.y - br.y) * (player.x - (player.x + direction_.x));
        
            if (den != 0)
            {
                float t_num = (tr.x - player.x) * (player.y - (player.y + direction_.y)) - (tr.y - player.y) * (player.x - (player.x + direction_.x));
                float u_num = (tr.x - player.x) * (tr.y - br.y) - (tr.y - player.y) * (tr.x - br.x);
                
                float t = t_num / den;
                float u = u_num / den;
                if (t <= 1 && t >= 0 && u >= 0)
                {
                    this->point_of_contact_.x = tr.x + t * (br.x - tr.x);
                    this->point_of_contact_.y = tr.y + t * (br.y - tr.y);

                    points.push_back(this->point_of_contact_);
                }
            }

            // bottom side
            den = (br.x - bl.x) * (player.y - (player.y + direction_.y))
                  - (br.y - bl.y) * (player.x - (player.x + direction_.x));
        
            if (den != 0)
            {
                float t_num = (br.x - player.x) * (player.y - (player.y + direction_.y)) - (br.y - player.y) * (player.x - (player.x + direction_.x));
                float u_num = (br.x - player.x) * (br.y - bl.y) - (br.y - player.y) * (br.x - bl.x);
                
                float t = t_num / den;
                float u = u_num / den;
                if (t <= 1 && t >= 0 && u >= 0)
                {
                    this->point_of_contact_.x = br.x + t * (bl.x - br.x);
                    this->point_of_contact_.y = br.y + t * (bl.y - br.y);

                    points.push_back(this->point_of_contact_);                    
                }
            }

            // if nothing has been found set contact point to start
            if (points.size() == 0)
                this->point_of_contact_ = *this->start_ + sf::Vector2f(200, 200);
            else
            {
                float min_d = INFINITY;
                for (auto p : points)
                {
                    float d = this->calculateMagnitude(p);
                    if (min_d > d)
                    {
                        min_d = d;
                        this->point_of_contact_ = p;
                    }
                }
                break;
            }
        }
    }
}