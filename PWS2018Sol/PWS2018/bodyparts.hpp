#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

//Limb representeerd 1 lidmaat
//Van punt b (begin) naar punt e (einde)
//Wanneer je m wil tekenen, doe eerst spawn() voor genereren!
//Huidige hy staat op 0.2 (dus 0.4 dik)
class Limb {
public:
	Limb() {}
	Limb(b2Vec2 b, float l, float a);
	~Limb();
	b2Body * body = NULL;
	b2Vec2 getBegin();
	b2Vec2 getEnd();
	void spawn();
	void draw();
	void update(b2Vec2, float);
	float length;
	float sAngle;
//s- = start-
//hx & hy = half x & half y
private: 
	b2Vec2 sBegin;
	b2Vec2 sEnd;
	b2Vec2 center;
	float hx;
	float hy = 0.2f;
	sf::RectangleShape shape;
};

class Head {
public:
	Head(b2Vec2 p);
	~Head();
	b2Body * body = NULL;
	b2Vec2 pos;
	void spawn();
	void draw();
	float r = 0.5;
private:
	sf::CircleShape shape;
};