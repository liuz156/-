#pragma once
#include "Base.hpp"

class Bullet : public Base {
public:
	Bullet(sf::Texture& texture, float x, float y, float speed, float directionX, float directionY,int health);
	void update(sf::RenderWindow& window,float dataTime) override;
private:
	sf::Vector2f direction;
};
