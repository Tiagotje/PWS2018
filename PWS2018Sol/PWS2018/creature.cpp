#include "creature.hpp"
#include "physics.hpp"
#include "NN.hpp"
#include "food.hpp"

#include <Box2D/Box2d.h>
#include <iostream>

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

void Creature::despawn()
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].despawn();
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
	nn->calculate(0.5f, -0.39f, 0.62f, 0.42f, -0.98f, 0.07f, -0.23f);
	
	for (int i = 0; i < OUTPUTSIZE && i < joints.size(); i++)
	{
		joints[i]->EnableMotor(true);
		joints[i]->SetMotorSpeed(nn->output[i] * 5);
		joints[i]->SetMaxMotorTorque(100);
	}
}

b2Vec2 Creature::getPos()
{
	return head.body->GetPosition();
}

void Creature::eat()
{
	std::cout << "nomnomnom" << std::endl;
	findFood();
}

void Creature::findFood()
{
	float lowest = 10000.0f;
	for (int i = 0; i<foodsize; i++)
	{
		Food f = food[i];
		if (!f.body->IsActive())
			continue;
		if ((getPos() - f.p).Length() < lowest)
		{
			lowest = (getPos() - f.p).Length();
			nearestFood = f.p;
		}
	}
	b2Vec2 p = nearestFood;
	std::cout << "nearest food at: " << p.x << ", " << p.y << std::endl;
}

void Creature::updateCreature()
{
	limbs.clear();
	for (int i = 0; i < nodes.size(); i++)
		nodes[i].updateCreature(this);
}

void Node::updateCreature(Creature * c)
{
	creature = c;
	c->limbs.push_back(this);
	for (int i = 0; i < nodes.size(); i++)
		nodes[i].updateCreature(c);
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
	c->limbs.push_back(this);
}

void Node::spawn(b2Body * bod)
{
	limb.spawn();
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bod, limb.body, limb.getBegin());
	jointDef.lowerAngle = -0.5 * b2_pi;
	jointDef.upperAngle = 0.5 * b2_pi;
	jointDef.enableLimit = true;
	joint = (b2RevoluteJoint *)Phys::world.CreateJoint(&jointDef);
	creature->joints.push_back(joint);

	for (int i = 0; i < nodes.size(); i++)
		nodes[i].spawn(limb.body);
}

void Node::despawn()
{
	limb.~Limb();
	for (int i = 0; i < nodes.size(); i++)
		nodes[i].despawn();
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

void Creature::updatePos()
{
	for (int i = 0; i < nodes.size(); i++)
		nodes[i].updatePos(b2Vec2(0,0));
}

void Node::updatePos(b2Vec2 b)
{
	limb.update(b, angle);
	for (int i = 0; i < nodes.size(); i++)
		nodes[i].updatePos(limb.getEnd());
}