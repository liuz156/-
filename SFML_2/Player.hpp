#pragma once
#include "Base.hpp"
#include "Bullet.hpp"

class Player : public Base {
public:
	Player(sf::Texture& texture, float x, float y, float speed,int health);
	void update(sf::RenderWindow& window,float dataTime) override;
};


