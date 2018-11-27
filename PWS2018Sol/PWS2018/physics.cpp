#include "physics.hpp"

#include <iostream>
#include <math.h>
#include "main.hpp"
#include "simstate.hpp"


b2Body * Phys::groundBody;
b2Vec2 grav(0.0f, -10.0f);
b2World Phys::world(grav);

bool initPhys = false;

float radToDeg = 57.29578f;

void Phys::initPhysics() {
	if (initPhys)
		return;
	initPhys = true;

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);
	groundBody = world.CreateBody(&groundBodyDef);

}

void Phys::updatePhysics(float fps) {
	int32 velocityIterations = 20; //6
	int32 positionIterations = 8;  //2
	world.Step(1.0f / fps, velocityIterations, positionIterations);
}

b2RevoluteJoint * Phys::revolute(b2Body * A, b2Body * B, b2Vec2 pos)
{
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(A, B, pos);
	return (b2RevoluteJoint*)Phys::world.CreateJoint(&jointDef);
}