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
	Creature();
	Creature(Creature *, Creature *);
	~Creature();
	Creature * parents[2];
	std::vector<Node*> nodes;
	std::vector<unsigned char> dominance;
	Head head = Head(b2Vec2(0, 0));
	std::vector<Node*> limbs;
	std::vector<b2RevoluteJoint*> joints;
	float energy = 500.0f;
	int foodcount = 0;
	void spawn();
	void draw();
	void calculate();
	void despawn();
	void updatePos();
	void eat(); //wat doen we als er wordt gegeten?
	void addNode(float l, float a, int n, float d);
	b2Vec2 getPos();
	void findFood();
	void updateCreature();
	void deleteNodes(Node * n);
	NN * nn;
private:
	b2Vec2 nearestFood;
};


class Node {
public:
	Node(b2Vec2 b, float l, float a, int n, Creature * c, float d);
	Limb limb;
	b2RevoluteJoint * joint;
	Creature * creature;
	std::vector<Node*> nodes;
	float dominance;
	float angle;
	void spawn(b2Body * parent);
	void draw();
	void addNode(float l, float a, int n, float d);
	void setSpeed(float v);
	void updatePos(b2Vec2);
	void despawn();
	int nodecount();
	int neuron; 
	void updateCreature(Creature *);
	bool contains(Node*);
};
