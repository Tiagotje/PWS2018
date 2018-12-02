#include "creature.hpp"
#include "physics.hpp"
#include "NN.hpp"
#include "food.hpp"
#include "genetics.hpp"

#include <Box2D/Box2d.h>
#include <iostream>
#include <algorithm>

Creature::Creature()
{
	nn = new NN(this);

	addNode(4, 0, 0, 0.36);
	addNode(4, (-0.5*b2_pi), 1, 0.42);
	addNode(4, b2_pi, 2, 0.83);
	nodes[0]->addNode(4, (-0.5*b2_pi), 3, 0.39);
	nodes[2]->addNode(4, (0.5*b2_pi), 4, 0.61);
}

Creature::Creature(Creature * a, Creature * b)
{
	parents[0] = new Creature(a);
	parents[1] = new Creature(b);
	nn = fenonn(a, b);
	Creature f = feno(a, b);
	f.updatePos();
	nodes = f.nodes;
	updateCreatureNodes();
}

Creature::Creature(Creature * c) {
	for (int i = 0; i < c->nodes.size(); i++)
		nodes.push_back(new Node(c->nodes[i]));
	nn = new NN(c->nn);
}

Creature::~Creature()
{
	//if(nn != NULL)
	//	delete nn;
}

void Creature::spawn()
{
	joints = std::vector<b2RevoluteJoint*>();
	head.spawn();
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->spawn(head.body);
	}
}

void Creature::despawn()
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->despawn();
	}
}


void Creature::draw()
{
	for (int i = 0; i<nodes.size(); i++)
		nodes[i]->draw();
	head.draw();
}

void Creature::addNode(float l, float a, int n, float d)
{
	nodes.push_back(new Node(b2Vec2(0,0), l, a, n, this, d));
}

void Creature::calculate()
{
	//richting, afstand
	b2Vec2 delta = (nearestFood - getPos());
	float distance = delta.Length();
	delta.x /= distance;
	nn->calculate(delta.x, distance, energy);
	
	for (int i = 0; i < nodes.size(); i++)
	{	
		nodes[i]->joint->EnableMotor(true);
		nodes[i]->joint->SetMotorSpeed(nn->output[nodes[i]->neuron] * 5);
		nodes[i]->joint->SetMaxMotorTorque(1000);
	}
	//energy
	energy -= 0.1f;
}

b2Vec2 Creature::getPos()
{
	return head.body->GetPosition();
}

void Creature::eat()
{
	energy +=  125.0f; 
	foodcount++;
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
}

void Creature::updateCreature()
{
	limbs.clear();
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->updateCreature(this);
}

void Creature::deleteNodes(Node * n)
{
	for (Node * c : n->nodes)
		deleteNodes(c);
	limbs.erase(std::remove(limbs.begin(), limbs.end(), n), limbs.end());
	for (int i = 0; i < nodes.size(); i++)
		if (nodes[i] == n) {
			nodes.erase(nodes.begin() + i);
			break;
		}
}

void Node::updateCreature(Creature * c)
{
	creature = c;
	c->limbs.push_back(this);
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->updateCreature(c);
}

void Node::addNode(float l, float a, int n, float d)
{
	nodes.push_back(new Node(limb.getEnd(), l, angle+a, n, creature, d));
}

Node::Node(b2Vec2 b, float l, float a, int n, Creature * c, float d)
{
	limb = Limb(b, l, a);
	angle = a;
	creature = c;
	neuron = n;
	c->limbs.push_back(this);
	dominance = d;
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
		nodes[i]->spawn(limb.body);
}

void Node::despawn()
{
	limb.~Limb();
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->despawn();
}

void Node::draw()
{
	limb.draw();
	for (int i = 0; i<nodes.size(); i++)
		nodes[i]->draw();
}

void Node::setSpeed(float v)
{
	joint->SetMotorSpeed(v);
	joint->EnableMotor(true);
	joint->SetMaxMotorTorque(2000);
}

void Creature::updatePos()
{
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->updatePos(b2Vec2(0,0));
}

void Node::updatePos(b2Vec2 b)
{
	limb.update(b, angle);
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->updatePos(limb.getEnd());
}

int Node::nodecount()
{
	if (nodes.size() == 0)
		return 0;
	int tot = nodes.size();
	for (Node * n : nodes)
		tot += n->nodecount();
	return tot;
}

bool Node::contains(Node* n)
{
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i] == n) 
			return true;
		if (nodes[i]->contains(n))
			return true;
	}
	return false;
}

void Creature::updateCreatureNodes()
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->deep = 0;
		nodes[i]->updateCreatureLimbs(this);
	}
}

void Node::updateCreatureLimbs(Creature * c) {
	creature = c;
	c->limbs.push_back(this);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->deep = deep + 1;
		nodes[i]->updateCreatureLimbs(c);
	}
}

Node::Node(Node * n) {
	dominance = n->dominance;
	angle = n->angle;
	neuron = n->neuron;
	limb = Limb(n->limb.getBegin(), n->limb.length, n->limb.sAngle);
	for (int i = 0; i < n->nodes.size(); i++)
		nodes.push_back( new Node( n->nodes[i] ) );
}

void Creature::checkfood(){
	for (int i = 0; i < foodsize; i++)
		if (food[i].check())
			eat();
}