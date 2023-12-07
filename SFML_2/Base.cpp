#include "Base.hpp"

Base::Base(sf::Texture& texture, float x, float y, float speed,int health)
    : health(health), speed(speed) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

Base::~Base() {}

void Base::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Base::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Base::getPosition() const {
    return sprite.getPosition();
}

void Base::setHealth(int health) {
    this->health = health;
}

int Base::getHealth() const {
    return health;
}

void Base::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
    }
    else if (health > 100) {
        health = 100;
    }
}

bool Base::isDestroyed() const {
    return health <= 0;
}

sf::Sprite Base::getSprite() const {
    return sprite;
}

void Base::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}
