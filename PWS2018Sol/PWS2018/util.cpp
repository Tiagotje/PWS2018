#include "util.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

sf::Font Util::dafont;
b2Body * Util::groundBody;
b2Body * Util::body;
b2Vec2 grav(0.0f, -10.0f);
b2World Util::world(grav);

//zodat de functie maar 1 keer laadt
bool initFont = false;
bool initPhys = false;

int iter = 0;

void Util::initFonts()
{
	if (initFont)
		return;
	initFont = true;
	int status;
	status = Util::dafont.loadFromFile("dafont.ttf");
	if (!status)
		std::cout << "Error with dafont.ttf" << std::endl;
}



void Util::initPhysics() {
	if (initPhys)
		return;
	initPhys = true;

	b2Vec2 grav(0.0f, -10.0f);
	world.SetGravity(grav);

	//Body in 4 steps
	//Define body with position, damping etc
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	groundBodyDef.fixedRotation = true;
	groundBodyDef.angle = 10.f;
	//Use world to create body
	groundBody = world.CreateBody(&groundBodyDef);
	//Define fixtures with shape
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	//Create fixture on body
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Object dat we gaan laten vallen
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 100.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
}

void Util::updatePhysics() {
	if (iter++ > 3000)
		return;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world.Step(1.0f / 500.0f, velocityIterations, positionIterations); 
	b2Vec2 pos = body->GetPosition();
	float32 angle = body->GetAngle();
	b2Vec2 v = body->GetLinearVelocity();
	if ((iter % 100) == 0) {
		std::cout << "X: " << pos.x << " Y: " << pos.y;
		std::cout << " Angle: " << angle << std::endl;
		std::cout << " V = (" << v.x << "," << v.y << ")" << std::endl;
	}
}