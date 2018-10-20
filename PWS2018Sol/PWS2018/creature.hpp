#pragma once

#include "main.hpp"
#include "bodyparts.hpp"
#include <vector>
//Creature =
//Head + Limb tree (+ NN)
class Creature {
	Creature::Creature();
	Head head = Head(b2Vec2(0, 0));
	std::vector<Node> nodes;
	std::vector<Limb*> limbs;
	void spawn();
	void draw();
	//void despawn();
public:
	void addNode();
};

class Node {
public:
	Limb limb;
	b2RevoluteJoint * joint;
	std::vector<Node> nodes;
	void setSpeed(float v);
	void spawn();
	void draw();
	//void despawn();
};



