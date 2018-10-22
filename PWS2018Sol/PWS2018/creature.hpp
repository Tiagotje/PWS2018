#pragma once

#include "main.hpp"
#include "bodyparts.hpp"
#include <vector>
#include <Box2D/Box2D.h>

class Node;

//Creature = Head + Limb tree (+ NN)
class Creature {
public:
	Head head = Head(b2Vec2(0, 0));
	std::vector<Limb*> limbs;
	Creature() {}
	std::vector<Node> nodes;
	void spawn();
	void draw();
	//void despawn();
	void addNode(float l, float a);
};


class Node {
public:
	Node(b2Vec2 b, float l, float a, Creature * c);
	Limb limb;
	b2RevoluteJoint * joint;
	Creature * creature;
	std::vector<Node> nodes;
	float angle;
	void spawn(b2Body * parent);
	void draw();
	void addNode(float l, float a);
	void setSpeed(float v);
	//void despawn();
};


