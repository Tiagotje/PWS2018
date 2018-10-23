#include "creature.hpp"
#include "physics.hpp"

#include <Box2D/Box2d.h>

Creature::Creature()
{
	nn = new NN(this);
	nn->initweights();
}

Creature::~Creature()
{
	delete nn;
}

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

void Creature::calculate()
{
	nn->calculate(0.5, -0.39, 0.62, 0.42, -0.98, 0.07, -0.23);
	
	for (int i = 0; i < OUTPUTSIZE && i < joints.size(); i++)
	{
		joints[i]->EnableMotor(true);
		joints[i]->SetMotorSpeed(nn->output[i] * 5);
		joints[i]->SetMaxMotorTorque(100);
	}
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
	jointDef.lowerAngle = -0.5 * b2_pi;
	jointDef.upperAngle = 0.5 * b2_pi;
	joint = (b2RevoluteJoint *)Phys::world.CreateJoint(&jointDef);
	creature->joints.push_back(joint);

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