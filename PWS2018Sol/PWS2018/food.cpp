#include "food.hpp"

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics.hpp"
#include "main.hpp"
#include "creature.hpp"

Food::Food(b2Vec2 pos)
{
	p = pos;

	shape = sf::CircleShape(10 * r);
	shape.setOrigin({ 10 * r, 10 * r });
	shape.setFillColor(sf::Color(200, 30, 30));

	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(p.x, p.y);
	body = Phys::world.CreateBody(&bd);
	b2CircleShape cs;
	cs.m_p.Set(0, 0);
	cs.m_radius = r;
	b2FixtureDef fd;
	fd.shape = &cs;
	body->CreateFixture(&fd);
}

void Food::draw()
{
	if (!body->IsActive())
		return;
	p = body->GetPosition();
	shape.setPosition(sf::Vector2f(10 * p.x, -10 * p.y));
	window.draw(shape);
}

bool Food::check()
{
	for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
	{
		if (ce->contact->IsTouching()) {
			body->SetActive(false);
			return true;
		}
	}

	return false;
}