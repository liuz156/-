#include "Player.hpp"

Player::Player(sf::Texture& texture, float x, float y, float speed,int health)
    : Base(texture, x, y, speed,health) {
    // Player-specific initialization
}

void Player::update(sf::RenderWindow& window,float dataTime) {
    // Player-specific update logic``

    float changespeed = 1.0f;
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
         changespeed = 0.4f;
     }
     else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
         changespeed = 1.2f;
     }
     dataTime *= changespeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        move(0.f,-speed * dataTime);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        move(0.f,speed * dataTime);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        move(-speed * dataTime, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        move(speed * dataTime, 0);
    }

    sf::Vector2f position = sprite.getPosition();
    sf::Vector2u windowsize = window.getSize();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    if (position.x < 0) {
        sprite.setPosition(0.f, position.y);
    }
    else if (position.y < 0) {
        sprite.setPosition(position.x, 0.f);
    }
    else if (position.x + spriteBounds.width > windowsize.x) {
        sprite.setPosition(windowsize.x - spriteBounds.width, position.y);
    }
    else if (position.y + spriteBounds.height > windowsize.y) {
        sprite.setPosition(position.x, windowsize.y - spriteBounds.height);
    }
}