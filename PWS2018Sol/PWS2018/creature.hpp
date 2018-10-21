#pragma once

#include "main.hpp"
#include "bodyparts.hpp"
#include <vector>
#include <Box2D/Box2D.h>

class Node {
public:
	Node(b2Vec2 b, float l, float a, b2Body * parent);
	Limb limb;
	b2RevoluteJoint * joint;
	std::vector<Node> nodes;
	b2Body * parent;
	void spawn();
	void draw();
	void setSpeed(float v);
	//void despawn();
};


//Creature =
//Head + Limb tree (+ NN)
class Creature {
	Head head = Head(b2Vec2(0, 0));
	std::vector<Node> nodes;
	std::vector<Limb*> limbs;
public:
	Creature() {}
	void spawn();
	void draw();
	//void despawn();
	void addNode(float l, float a);
};



