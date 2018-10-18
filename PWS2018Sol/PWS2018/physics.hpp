#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

namespace Phys {
	extern b2Body * groundBody;
	extern b2World world;
	void initPhysics();
	void updatePhysics(float fps);
	b2Body * genBall(float x, float y, float radius);
	b2Body * genRod(float hw, float hy, b2Vec2 center, float angle);
}

class BallObject {
public:
	b2Body * body;
	sf::CircleShape shape;
	BallObject(float x, float y);
	void force(float x, float y);
	void draw();
};

class RodObject {
public:
	b2Body * body;
	sf::RectangleShape shape;
	float dx;
	float dy;
	RodObject(float hx, float hy, b2Vec2 center, float angle);
	void draw();
};