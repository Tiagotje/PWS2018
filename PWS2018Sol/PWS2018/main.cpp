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
#include "food.hpp"
#include "physics.hpp"


sf::RenderWindow window;

State * state;
float FPS = 1000;
int iter = 0;
const float timestep = 60;
const int foodsize = 200;
b2Vec2 foodpos[200];
Food food[200];

bool pauze = false;
bool slow = false;

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
	window.create(sf::VideoMode(1600, 900), "Evolutie simulatie", sf::Style::Titlebar | sf::Style::Close);
	state = new MenuState();

	sf::Clock clock;
	float frame = 1;

	while (window.isOpen())
	{
		events();
		
		if (!pauze) {
			state->calculate();
			Phys::updatePhysics(timestep);
		}

		if (slow | clock.getElapsedTime() > sf::milliseconds(15)) {
			frame++;
			state->draw();
			window.display();
			clock.restart();
		}

		if(slow)
			sf::sleep(sf::milliseconds(10)-clock.restart());
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
