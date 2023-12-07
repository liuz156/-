#pragma once
#include <SFML/Graphics.hpp>

class Base {
protected:
	sf::Sprite sprite;
	float speed;
	int health;
public:
    Base(sf::Texture& texture, float x, float y, float speed,int health = 100);
    virtual ~Base();

    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setHealth(int health);
    int getHealth() const;
    void takeDamage(int damage);
    bool isDestroyed() const;

    sf::Sprite getSprite()const;

    void move(float offsetX, float offsetY);

    virtual void update(sf::RenderWindow& window,float dataTime) = 0;

};
