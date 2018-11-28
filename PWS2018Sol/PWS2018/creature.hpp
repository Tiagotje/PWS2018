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
	Creature(Creature *); // copy constr
	~Creature();
	Creature * parents[2];
	std::vector<Node*> nodes;
	Head head = Head(b2Vec2(0, 0));
	std::vector<Node*> limbs;
	std::vector<b2RevoluteJoint*> joints;
	float energy = 1000.0f; //500 -> 2.000 -> 1.000
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
	void updateCreatureNodes();
	NN * nn;
private:
	b2Vec2 nearestFood;
};


class Node {
public:
	Node(Node * n);
	Node(b2Vec2 b, float l, float a, int n, Creature * c, float d);
	Limb limb;
	b2RevoluteJoint * joint;
	Creature * creature;
	std::vector<Node*> nodes;
	float dominance;
	float angle;
	int deep = 0;
	void spawn(b2Body * parent);
	void draw();
	void addNode(float l, float a, int n, float d);
	void setSpeed(float v);
	void updatePos(b2Vec2);
	void updateCreature(Creature * c);
	void despawn();
	int nodecount();
	int neuron; 
	void updateCreatureLimbs(Creature *);
	bool contains(Node*);
};
