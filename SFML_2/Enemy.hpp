#pragma once
#include "Base.hpp"
#include "Bullet.hpp"

class Enemy : public Base {
public:
	Enemy(sf::Texture& texture, float x, float y, float speed,int health);
	void update(sf::RenderWindow& window,float dataTIme) override;
};
