#pragma once

#include "main.hpp"
#include "bodyparts.hpp"
#include "NN.hpp"

#include <vector>
#include <Box2D/Box2D.h>

class Node;
class NN;

//Creature = Head + Limb tree (+ NN)
class Creature {
public:
	Head head = Head(b2Vec2(0, 0));
	std::vector<Limb*> limbs;
	std::vector<b2RevoluteJoint*> joints;
	Creature();
	~Creature();
	std::vector<Node> nodes;
	void spawn();
	void draw();
	void calculate();
	void despawn();
	void eat(); //wat doen we als er wordt gegeten?
	void addNode(float l, float a);
	b2Vec2 getPos();
	void findFood();
private:
	b2Vec2 nearestFood;
	NN * nn;
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
	void despawn();
};


