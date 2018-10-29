#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Food {
public:
	Food() {};
	Food(b2Vec2 pos);
	b2Vec2 p;
	float r = 1.0;
	sf::CircleShape shape;
	b2Body * body;
	void draw();
	void check();
};