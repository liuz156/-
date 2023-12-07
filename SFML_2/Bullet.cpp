#include "Bullet.hpp"

Bullet::Bullet(sf::Texture& texture, float x, float y, float speed, float directionX, float directionY,int health)
    : Base(texture, x, y, speed,health) {
    // Bullet-specific initialization
    sf::Vector2f dist(x, y);
    sf::Vector2f dist2(directionX, directionY);
    sf::Vector2f d = dist2 - dist;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    if (len != 0) d /= len;
    direction = sf::Vector2f(d);

}

void Bullet::update(sf::RenderWindow& window, float dataTime) {
    // Bullet-specific update logic
    sprite.move(direction* speed);
}