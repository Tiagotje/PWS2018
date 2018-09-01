#include "main.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <string>


#include "state.hpp"
#include "simstate.hpp"
#include "util.hpp"
#include "physics.hpp"


sf::RenderWindow window;

State * state;
float FPS = 1000;
int iter = 0;

//VERGEET NIET!!
//SCHAAL VERSCHIL, WEGENS WISKUNDE SHIT
//1 UNIT IN BOX2D = 10 UNITS IN SFML!!
//EN Y-AXIS GESPIEGELD
//Dus  Box2D (8, 50) = (80, -500) in SFML

int main()
{
	//Laad alle lettertypes aan het begin
	Util::initFonts();
	Phys::initPhysics();
	window.create(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	state = new SimState();

	sf::Clock clock;
	float lastTime = 0;

	while (window.isOpen())
	{
		state->events();
		state->calculate();
		Phys::updatePhysics(7000.0f);
		state->draw();
		window.display();

		if ((iter % 100) == 0) {
			float curr = clock.restart().asSeconds();
			FPS = (100.0f / (curr));
		}
		iter++;
	}

	return 0;
}