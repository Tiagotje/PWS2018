#include "creature.hpp"
#include "physics.hpp"

#include <Box2D/Box2d.h>

void Creature::spawn()
{
	head.spawn();
	for (Node n : nodes) {
		n.parent = head.body;
		n.spawn();
	}
}


void Creature::draw()
{
	for (Node n : nodes)
		n.draw();
	head.draw();
}

void Creature::addNode(float l, float a)
{
	//beginpunt op rand van cirkel
	b2Vec2 b = b2Vec2(sin(a) * head.r, cos(a) * head.r);
	nodes.push_back(Node(b, l, a, head.body));
}

Node::Node(b2Vec2 b, float l, float a, b2Body * bod)
{
	limb = Limb(b, l, a);
	parent = bod;
}

void Node::spawn()
{
	limb.spawn();
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(parent, limb.body, limb.getBegin());
	joint = (b2RevoluteJoint *)Phys::world.CreateJoint(&jointDef);

	//for (Node n : nodes)
		//n.spawn();
}

void Node::draw()
{
	limb.draw();
	for (Node n : nodes)
		n.draw();
}

void Node::setSpeed(float v)
{
	joint->SetMotorSpeed(v);
	joint->EnableMotor(true);
	joint->SetMaxMotorTorque(100);
}