#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

namespace Phys {
	extern b2Body * groundBody;
	extern b2World world;
	void initPhysics();
	void updatePhysics(float fps);
	b2Body * genBall(float x, float y, float radius);
}

class BallObject {
	b2Body * body;
	sf::CircleShape shape;
public:
	BallObject(float x, float y);
	void force(float x, float y);
	void draw();
};