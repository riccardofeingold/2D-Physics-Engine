#include "../../include/no_gravity_game_ws/Physics2D/ray.hpp"

using namespace Physics2D;

Ray::Ray(Rigidbody*& body, float angle)
    : body_(body),
      angle_(angle)
{
    float rad = angle_ * M_PI / 180;
    this->direction_ = sf::Vector2f(std::cos(rad), std::sin(rad));
    this->line_ = sf::RectangleShape(sf::Vector2f(0, THICKNESS));
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(Vector2Converter::toSFVector2f(this->body_->getPosition()));
    this->line_.setFillColor(sf::Color::White);

}

Ray::~Ray() = default;

// getters
const sf::RectangleShape& Ray::getLineShape() const { return this->line_; }
const float Ray::getDistance() const { return this->distance_; }
const float Ray::getAngle() const { return this->angle_; }

float Ray::calculateMagnitude(sf::Vector2f& end_point)
{
    sf::Vector2f line_vector = Vector2Converter::toSFVector2f(this->body_->getPosition()) - end_point;
    return std::sqrt(line_vector.x * line_vector.x + line_vector.y * line_vector.y);
}

void Ray::draw()
{
    this->distance_ = this->calculateMagnitude(this->point_of_contact_);
    line_.setOrigin(sf::Vector2f(0, line_.getSize().y/2));
    line_.setPosition(Vector2Converter::toSFVector2f(this->body_->getPosition()));

    if (this->distance_ <= MAX_DISTANCE)
        line_.setSize(sf::Vector2f(this->distance_, THICKNESS));
    else
        line_.setSize(sf::Vector2f(MAX_DISTANCE, THICKNESS));
        
    line_.rotate(this->angle_);
}

void Ray::castRay(World2D& w)
{
    Vector2f player = this->body_->getPosition();

    std::vector<sf::Vector2f> global_points;
    // wikipedia article about line intersection: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    for (int i = 0; i < w.getBodyCount(); ++i)
    {
        Rigidbody* b;
        w.getBody(i, b);

        if (this->body_ != b)
        {
            if (b->shape_type == ShapeType::Circle)
            {
                Vector2f point;
                if (this->lineCircleIntersection(player, b, point))
                {
                    this->point_of_contact_ = Vector2Converter::toSFVector2f(point);
                    global_points.push_back(this->point_of_contact_);
                }
            } else if (b->shape_type == ShapeType::Box)
            {
                std::vector<sf::Vector2f> points;

                const Vector2f& tl = b->getTransformedVertices()[0];
                const Vector2f& tr = b->getTransformedVertices()[1];
                const Vector2f& br = b->getTransformedVertices()[2];
                const Vector2f& bl = b->getTransformedVertices()[3];

                this->checkLeftSide(player, tl, tr, br, bl, points);
                this->checkTopSide(player, tl, tr, br, bl, points);
                this->checkRightSide(player, tl, tr, br, bl, points);
                this->checkBottomSide(player, tl, tr, br, bl, points);                

                // if nothing has been found set contact point to start
                if (points.size() == 0)
                    this->point_of_contact_ = Vector2Converter::toSFVector2f(this->body_->getPosition()) + sf::Vector2f(this->RANGE, this->RANGE);
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
                    global_points.push_back(this->point_of_contact_);
                }
            }
        }
    }

    float min_d = INFINITY;
    for (auto p : global_points)
    {
        float d = this->calculateMagnitude(p);
        if (min_d > d)
        {
            min_d = d;
            this->point_of_contact_ = p;
        }
    }

    // check if object is in range
    if (Math2D::distance_squared(Vector2Converter::toPhysics2DVector2f(this->point_of_contact_), player) > RANGE * RANGE)
    {
        this->point_of_contact_ = Vector2Converter::toSFVector2f(this->body_->getPosition()) + sf::Vector2f(RANGE, RANGE);
    }

}

bool Ray::lineCircleIntersection(const Vector2f& player, Rigidbody* const & body, Vector2f& point)
{
    Vector2f diff_pos = body->getPosition() - player;
    Vector2f direction = Vector2Converter::toPhysics2DVector2f(this->direction_);
    float dot = Math2D::dot(direction, diff_pos);

    if (dot < 0)
        return false;

    float xpm = player.x - body->getPosition().x;
    float ypm = player.y - body->getPosition().y;
    float b = 2 * (direction.x * xpm + direction.y * ypm);
    float c = xpm * xpm + ypm * ypm - body->radius * body->radius;

    float discriminant = b * b - 4 * c;
    if (discriminant < 0)
        return false;
    else if (discriminant == 0)
    {
        float lambda = -b / 2;
        point.x = lambda * direction.x + player.x;
        point.y = lambda * direction.y + player.y;
    } else if (discriminant > 0)
    {
        float lambda1 = (-b + std::sqrt(discriminant)) / 2;
        float lambda2 = (-b - std::sqrt(discriminant)) / 2;

        Vector2f point1;
        Vector2f point2;

        point1.x = lambda1 * direction.x + player.x;
        point1.y = lambda1 * direction.y + player.y;
        point2.x = lambda2 * direction.x + player.x;
        point2.y = lambda2 * direction.y + player.y;

        float d1 = Math2D::distance_squared(point1, player);
        float d2 = Math2D::distance_squared(point2, player);

        if (d1 < d2)
            point = point1;
        else
            point = point2;
    }

    return true;
}

void Ray::checkLeftSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points)
{
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
}

void Ray::checkRightSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points)
{
    float den = (tr.x - br.x) * (player.y - (player.y + direction_.y))
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
}

void Ray::checkBottomSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points)
{
    float den = (br.x - bl.x) * (player.y - (player.y + direction_.y))
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
}

void Ray::checkTopSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points)
{
    float den = (tl.x - tr.x) * (player.y - (player.y + direction_.y))
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
}

float Ray::getDistance()
{
    return Math2D::distance(this->body_->getPosition(), Vector2Converter::toPhysics2DVector2f(this->point_of_contact_));
}