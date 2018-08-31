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

//VERGEET NIET!!
//SCHAAL VERSCHIL, WEGENS WISKUNDE SHIT
//1 UNIT IN BOX2D = 100 UNITS IN SFML!!

int main()
{
	//Laad alle lettertypes aan het begin
	Util::initFonts();
	window.create(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	state = new SimState();

	Phys::initPhysics();

	while (window.isOpen())
	{
		state->events();
		state->calculate();
		Phys::updatePhysics(500.0f);
		state->draw();
		window.display();
	}

	return 0;
}