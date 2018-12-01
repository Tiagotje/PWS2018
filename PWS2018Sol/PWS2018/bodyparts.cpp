#include "bodyparts.hpp"

#include <math.h>
#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "physics.hpp"
#include "main.hpp"


Limb::Limb(b2Vec2 b, float l, float a)
{
	sAngle = a;
	sBegin = b;
	length = l;

	//sin = y/l  cos = x/l
	sEnd = b2Vec2(b.x + cos(a)*l, b.y + sin(a)*l);

	center = 0.5 * (sBegin + sEnd);

	hx = sqrt(pow(center.x-b.x, 2) +
			  pow(center.y-b.y, 2));
}

void Limb::update(b2Vec2 b, float a)
{
	sBegin = b;
	float l = length;
	sEnd = b2Vec2(b.x + cos(a)*l, b.y + sin(a)*l);
	sAngle = a;
	center = 0.5 * (sBegin + sEnd);
	hx = sqrt(pow(center.x-b.x, 2) +
			  pow(center.y-b.y, 2));
}

Limb::~Limb()
{
	if (body != NULL) {
		body->GetWorld()->DestroyBody(body);
		body = NULL;
	}
}

void Limb::spawn()
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(center.x, center.y);
	bd.angle = sAngle;
	body = Phys::world.CreateBody(&bd);
	b2PolygonShape poly;
	poly.SetAsBox(hx, hy);
	b2FixtureDef fd;
	fd.shape = &poly;
	fd.density = 1.0f;
	fd.friction = 0.95f;
	fd.restitution = 0.05f;
	fd.filter.categoryBits = 2;
	fd.filter.maskBits = 1;
	body->CreateFixture(&fd);

	shape = sf::RectangleShape(sf::Vector2f(20 * hx, 20 * hy));
	shape.setFillColor(sf::Color(10, 30, 210));
}

void Limb::draw()
{
	center = body->GetPosition();
	float angle = body->GetAngle();
	float radToDeg = 57.29578f;
	float sfmlAngle = -angle * radToDeg; 
	shape.setRotation(sfmlAngle);

	//rotate around point
	//xR = x cos A - y sin A
	//yR = x sin A + y cos A
	float dx = -hx;
	float xR = dx * cos(angle) - hy * sin(angle) + center.x;
	float yR = dx * sin(angle) + hy * cos(angle) + center.y;
	shape.setPosition(10 * xR, -10 * yR);

	window.draw(shape);
}

b2Vec2 Limb::getEnd()
{
	return sEnd;
}

b2Vec2 Limb::getBegin()
{
	return sBegin;
}

Head::Head(b2Vec2 p) {
	pos = p;
	shape = sf::CircleShape(10 * r);
	shape.setOrigin({ 10 * r, 10 * r });
	shape.setFillColor(sf::Color(50, 200, 200));
}

Head::~Head() {
	if(body != NULL && body->GetWorld() != NULL)
		body->GetWorld()->DestroyBody(body);
}


//bodydef = eigenschappen voor simulatie
//circleshape = de vorm
//fixture = materiaal
void Head::spawn()
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);
	bd.fixedRotation = false;
	body = Phys::world.CreateBody(&bd);
	b2CircleShape cs;
	cs.m_p.Set(0, 0);
	cs.m_radius = r;
	b2FixtureDef fd;
	fd.shape = &cs;
	fd.density = 0.3f;
	fd.friction = 0.95f;
	fd.restitution = 0.05f;
	fd.filter.categoryBits = 2;
	fd.filter.maskBits = 1;
	body->CreateFixture(&fd);
}

void Head::draw()
{
	pos = body->GetPosition();
	shape.setPosition(sf::Vector2f(10 * pos.x, -10 * pos.y));
	window.draw(shape);
}