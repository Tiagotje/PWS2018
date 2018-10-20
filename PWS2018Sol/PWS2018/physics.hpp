#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>

namespace Phys {
	extern b2Body * groundBody;
	extern b2World world;
	void initPhysics();
	void updatePhysics(float fps);
	b2RevoluteJoint * revolute(b2Body * A, b2Body * B, b2Vec2 pos);
}