#include "Enemy.hpp"

Enemy::Enemy(sf::Texture& texture, float x, float y, float speed,int health)
    : Base(texture, x, y, speed,health) {
    // Enemy-specific initialization
}

void Enemy::update(sf::RenderWindow& window, float dataTime) {
    // Enemy-specific update logic
    sprite.move(0.f, speed * dataTime);

}