#include "physics.hpp"

#include <iostream>
#include "main.hpp"
#include "simstate.hpp"

b2Body * Phys::groundBody;
b2Vec2 grav(0.0f, -10.0f);
b2World Phys::world(grav);

bool initPhys = false;
int iter = 0;

void Phys::initPhysics() {
	if (initPhys)
		return;
	initPhys = true;

	b2Vec2 grav(0.0f, -10.0f);
	world.SetGravity(grav);

	//Body in 4 steps
	//Define body with position, damping etc
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);
	groundBodyDef.fixedRotation = true;
	//groundBodyDef.angle = 10.f;
	//Use world to create body
	groundBody = world.CreateBody(&groundBodyDef);
	//Define fixtures with shape
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	//Create fixture on body
	groundBody->CreateFixture(&groundBox, 0.0f);
}

void Phys::updatePhysics(float fps) {
	int32 velocityIterations = 10; //6
	int32 positionIterations = 4;  //2

	world.Step(1.0f / fps, velocityIterations, positionIterations);

	iter++;
}

//Body = object,  bodydef = hoe het werkt & plaats
//shape = vorm,   fixture = natk eigenschappen (+ vorm)
b2Body * Phys::genBall(float x, float y, float r) {
	b2Body * ball;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x, y);
	ball = world.CreateBody(&bd);
	b2CircleShape cs;
	cs.m_p.Set(0, 0);
	cs.m_radius = r;
	b2FixtureDef fd;
	fd.shape = &cs;
	fd.density = 1.0f;
	fd.friction = 0.3f;
	ball->CreateFixture(&fd);
	return ball;
}

BallObject::BallObject(float x, float y){
	body = Phys::genBall(x, y, 1);
	shape = sf::CircleShape(10);
	shape.setFillColor(sf::Color(250, 50, 50));
}

void BallObject::draw() {
	b2Vec2 pos = body->GetPosition();
	shape.setPosition(sf::Vector2f(pos.x , pos.y));
	window.draw(shape);
}