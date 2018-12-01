#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "food.hpp"
#include "state.hpp"


//extern, omdat de definitie in main.cpp moet staan
extern sf::RenderWindow window;
extern float FPS;
extern int iter;
extern State * state;
extern const float timestep;

extern const int foodsize;
extern b2Vec2 foodpos[];
extern Food food[];

extern bool pauze;
extern bool slow;