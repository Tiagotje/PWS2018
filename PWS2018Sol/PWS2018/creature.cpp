#include "creature.hpp"

#include <math.h>
#include "physics.hpp"

float Limb::angle()
{
	float dx = end.x - begin.x;
	float dy = end.y - begin.y;
	float halfpi = 1.57079633;
	return (dx == 0 ? halfpi : atan(dy / dx));
}

Limb::Limb(b2Vec2 b, b2Vec2 e)
{
	begin = b;
	end = e;
	center = b2Vec2(0.5*(b.x + e.x),
					0.5*(b.y + e.y));
	hx = sqrt(pow(center.x-b.x, 2) +
			  pow(center.y-b.y, 2));
}

Limb::~Limb()
{
	body->GetWorld()->DestroyBody(body);
}

void Limb::spawn()
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(center.x, center.y);
	bd.angle = angle();
	body = Phys::world.CreateBody(&bd);
	b2PolygonShape poly;
	poly.SetAsBox(hx, hy);
	b2FixtureDef fd;
	fd.shape = &poly;
	fd.density = 1.0f;
	fd.friction = 0.5f;
	fd.restitution = 0.05f;
	body->CreateFixture(&fd);

	shape = sf::RectangleShape(sf::Vector2f(20 * hx, 20 * hy));
	shape.setFillColor(sf::Color(10, 30, 210));
}

void Limb::draw()
{
	b2Vec2 pos = body->GetPosition();
	float angle = body->GetAngle();
	float radToDeg = 57.29578;
	float sfmlAngle = -angle * radToDeg; 
	shape.setRotation(sfmlAngle);

	//rotate around point,  thanks Mehrdad Afshari (stackoverflow)
	//xR = x cos A - y sin A
	//yR = x sin A + y cos A

	float xR = (-1 * hx * cos(angle)) - ( hy * sin(angle)) + pos.x;
	float yR = (-1 * hx * sin(angle)) + ( hy * cos(angle)) + pos.y;
	shape.setPosition(10 * xR, -10 * yR);

	window.draw(shape);
}