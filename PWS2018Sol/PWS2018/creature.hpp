#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "main.hpp"
//Creature = Neural Network + tree of pointers to limbs + head
class Creature {
	//general variables


	//graphics

};


//Limb representeerd 1 lidmaat
//Van punt b (begin) naar punt e (einde)
//Wanneer je m wil tekenen, doe eerst spawn() voor genereren!
//Huidige hy staat op 0.2 (dus 0.4 dik)
class Limb {
public:
	Limb(b2Vec2 b, b2Vec2 e);
	~Limb();
	b2Body * body;
	void spawn();
	void draw();
private:
	b2Vec2 begin;
	b2Vec2 end;
	b2Vec2 center;
	float hx;
	float hy = 0.2;
	float angle();
	sf::RectangleShape shape;
};

