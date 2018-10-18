#include "physics.hpp"

#include <iostream>
#include <math.h>
#include "main.hpp"
#include "simstate.hpp"


b2Body * Phys::groundBody;
b2Vec2 grav(0.0f, -10.0f);
b2World Phys::world(grav);
b2RevoluteJoint* Phys::joint;

bool initPhys = false;

float radToDeg = 57.29578;

void Phys::initPhysics() {
	if (initPhys)
		return;
	initPhys = true;

	world.SetGravity(grav);

	//Body in 4 steps
	//Define body with position, damping etc
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);
	//Use world to create body
	groundBody = world.CreateBody(&groundBodyDef);
}

void Phys::updatePhysics(float fps) {
	int32 velocityIterations = 10; //6
	int32 positionIterations = 4;  //2
	world.Step(1.0f / fps, velocityIterations, positionIterations);
	if (iter % (7000) == 0)
		std::cout << iter << " FPS: " << FPS << std::endl;
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
	fd.restitution = 0.2f;
	ball->CreateFixture(&fd);
	return ball;
}

BallObject::BallObject(float x, float y){
	body = Phys::genBall(x, y, 1);
	shape = sf::CircleShape(10);
	shape.setOrigin({10,10});
	shape.setFillColor(sf::Color(250, 50, 50));
}

void BallObject::draw() {
	b2Vec2 pos = body->GetPosition();
	shape.setPosition(sf::Vector2f(10 * pos.x , -10 * pos.y));
	window.draw(shape);
	//std::cout << "X: " << pos.x << " Y: " << pos.y << std::endl;
}

void BallObject::force(float x, float y) {
	body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
}

b2Body * Phys::genRod(float hx, float hy, b2Vec2 center, float angle) {
	b2Body * rod;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(center.x, center.y);
	bd.angle = angle;
	rod = world.CreateBody(&bd);
	b2PolygonShape poly;
	poly.SetAsBox(hx, hy);
	b2FixtureDef fd;
	fd.shape = &poly;
	fd.density = 1.0f;
	fd.friction = 0.05f;
	fd.restitution = 0.1f;
	rod->CreateFixture(&fd);
	return rod;
}

RodObject::RodObject(float _dx, float _dy, b2Vec2 center, float angle)
{
	dx = _dx;  dy = _dy;
	body = Phys::genRod(0.5*dx , 0.5*dy, center, angle);
	shape = sf::RectangleShape(sf::Vector2f(10 * dx, 10 * dy));
	shape.setFillColor(sf::Color(10, 30, 210));
}

void RodObject::draw() 
{
	b2Vec2 pos = body->GetPosition();
	float angle = body->GetAngle();
	float sfmlAngle = -angle * radToDeg; //omfg
	shape.setRotation(sfmlAngle);

	//rotate around point,  thanks Mehrdad Afshari (stackoverflow)
	//xR = x cos A - y sin A
	//yR = x sin A + y cos A

	float xR = (-0.5 * dx * cos(angle)) - ( 0.5 * dy * sin(angle)) + pos.x;
	float yR = (-0.5 * dx * sin(angle)) + ( 0.5 * dy * cos(angle)) + pos.y;
	shape.setPosition(10 * xR, -10 * yR);

	window.draw(shape);
}

void Phys::revolute(b2Body * A, b2Body * B, b2Vec2 pos)
{
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(A, B, pos);
	Phys::joint = (b2RevoluteJoint*)Phys::world.CreateJoint(&jointDef);

}