#include "main.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <string>


#include "state.hpp"
#include "menustate.hpp"
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

//Coords Box2D object = midden
//Coords SFML object = linksboven

void events();

int main()
{
	//Laad alle lettertypes aan het begin
	Util::initFonts();
	Phys::initPhysics();
	window.create(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	state = new MenuState();

	sf::Clock clock;
	float lastTime = 0;

	while (window.isOpen())
	{
		events();
		state->calculate();
		//norm = 5000
		Phys::updatePhysics(500.0f);
		if (iter % 1 == 0) {
			state->draw();
			window.display();
		}

		if ((iter % 100) == 0) {
			float curr = clock.restart().asSeconds();
			FPS = (100.0f / (curr));
		}

		iter++;
	}

	return 0;
}

void events()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			window.close();
		else {
			state->events(ev);
		}
	}
}


//Voorbeeld abstract classes voor mr. raaf
// Car * car
// 
// class Volkswagen : public Car { -- stuff -- }
// class Skoda : public Car { -- stuff -- }

// class Car{
// public:
//		virtual int kosten(int a, int b) = 0;
// }

// car = new Volkswagen();
// car->kosten(a,b);
// (*car).kosten(a,b);
// delete car;
// car = new Skoda();
// car->kosten(a,b); 
