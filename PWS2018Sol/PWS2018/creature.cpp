#include "creature.hpp"
#include "physics.hpp"

#include <Box2D/Box2d.h>

void Creature::spawn()
{
	head.spawn();
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].spawn(head.body);
	}
}


void Creature::draw()
{
	for (int i = 0; i<nodes.size(); i++)
		nodes[i].draw();
	head.draw();
}

void Creature::addNode(float l, float a)
{
	nodes.push_back(Node(b2Vec2(0,0), l, a, this));
}

void Node::addNode(float l, float a)
{
	nodes.push_back(Node(limb.getEnd(), l, angle+a, creature));
}

Node::Node(b2Vec2 b, float l, float a, Creature * c)
{
	limb = Limb(b, l, a);
	angle = a;
	creature = c;
}

void Node::spawn(b2Body * bod)
{
	limb.spawn();
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bod, limb.body, limb.getBegin());
	joint = (b2RevoluteJoint *)Phys::world.CreateJoint(&jointDef);

	for (int i = 0; i < nodes.size(); i++)
		nodes[i].spawn(limb.body);
	creature->limbs.push_back(&limb);
}

void Node::draw()
{
	limb.draw();
	for (int i = 0; i<nodes.size(); i++)
		nodes[i].draw();
}

void Node::setSpeed(float v)
{
	joint->SetMotorSpeed(v);
	joint->EnableMotor(true);
	joint->SetMaxMotorTorque(1000);
}