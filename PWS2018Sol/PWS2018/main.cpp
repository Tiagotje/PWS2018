#include "main.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Box2D/Box2D.h"
#include "state.hpp"
#include "simstate.hpp"
#include "fonts.hpp"

sf::RenderWindow window;

State * state;

int main()
{
	//Laad alle lettertypes aan het begin
	Util::initFonts();

	window.create(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

	state = new SimState();

	while (window.isOpen())
	{
		state->events();
		state->calculate();
		state->draw();
		window.display();
	}

	return 0;
}