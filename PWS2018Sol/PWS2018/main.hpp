#pragma once

#include <SFML/Graphics.hpp>
#include "state.hpp"

//extern, omdat de definitie in main.cpp moet staan
extern sf::RenderWindow window;
extern float FPS;
extern int iter;
extern State * state;
