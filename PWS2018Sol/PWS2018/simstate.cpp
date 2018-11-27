#include "simstate.hpp"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "main.hpp"
#include "terrain.hpp"
#include "util.hpp"
#include "button.hpp"
#include "creature.hpp"
#include "food.hpp"
#include "genetics.hpp"

SimState * ss;

void dab();

int genID = 0;
float gensum = 0.0f;

SimState::SimState(){
	Util::initFonts();
	//reset geeft aan welk deel van de view op het scherm moet komen
	//Viewport geeft in fracties aan waar op het scherm deze getekend moet worden
	upperView.reset(sf::FloatRect(0, 0, 1600, 45));
	upperView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 0.05f));
	mapView.reset(sf::FloatRect(-1000, -300, 2000, 1000));
	mapView.setViewport(sf::FloatRect(0.0f, 0.05f, 1.0f, 0.85f));
	lowerView.reset(sf::FloatRect(0, 0, 1600, 90));
	lowerView.setViewport(sf::FloatRect(0.0f, 0.9f, 1.0f, 0.1f));

	//Genereert de grond; 
	ground = TerrainGen();

	//De testknop
	but.setView(upperView);
	but.setFont(Util::dafont);
	but.setText("NEXT", 25);
	but.setSize(sf::FloatRect(150, 5, 100, 35));
	ss = this;
	but.setFunc(dab);
	but.setColor(sf::Color::Black);
	but.setTextPos(10, 1);

	pop = genPopulation();

	std::cout << "cID = " << creatureID << std::endl;

	active = pop[creatureID];
	active->spawn();

	for (int i = 0; i < foodsize; i++)
		food[i] = Food(foodpos[i]);

	active->findFood();
}

void dab() {
	ss->nextCreature();
}

//Nu nog leeg...h
void SimState::calculate() 
{  
	if (active->energy < 0.0f)
		nextCreature();
	if (active->getPos().x < -20.0f)
		nextCreature();
	active->calculate();
	for (int i = 0; i < foodsize; i++)
		food[i].check();
}


void SimState::draw()
{
	window.clear(sf::Color(75, 100, 255));

	//tekent map
	window.setView(mapView);
	window.draw(ground);

	active->draw();
	for (int i = 0; i < foodsize; i++)
		food[i].draw();

	//Tekent onderkant
	window.setView(lowerView);
	sf::RectangleShape rect(sf::Vector2f(1600, 90));
	rect.setFillColor(sf::Color(80, 80, 80));
	window.draw(rect);

	//Tekent bovenkant
	window.setView(upperView);
	rect.setSize(sf::Vector2f(1600, 45));
	window.draw(rect);
	but.draw();
}

void SimState::nextCreature()
{
	b2Vec2 p = active->getPos();
	int f;
	if (p.x < 0)
		f = 0;
	else
		f = p.x + 20 * active->foodcount;
	fitness[creatureID] = f;
	//std::cout << "c: " << creatureID << " f: " << f << std::endl;
	gensum += f;
	active->despawn();

	Phys::updatePhysics(timestep);

	for (int i = 0; i < foodsize; i++)
		food[i].body->SetActive(true);

	creatureID++;
	if (creatureID < POPSIZE) {
		active = pop[creatureID];
		active->spawn();
		active->findFood();
	} else {
		std::cout << "Gemiddelde fitheid: " << gensum / POPSIZE << std::endl;
		gensum = 0.0f;
		//std::cout << "New gen: " << genID++ << std::endl;
		pop = genNewPop(pop, fitness);
		creatureID = 0;
		active = pop[0];
		active->spawn();
		active->findFood();
	}
}

void SimState::events(sf::Event ev)
{
	//Wanneer op kruisje geklikt wordt
	if (ev.type == sf::Event::Closed)
		window.close();

	//Naar links of naar rechts bewegen met de view
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		mapView.setCenter(mapView.getCenter() - sf::Vector2f(100, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mapView.setCenter(mapView.getCenter() + sf::Vector2f(100, 0));
	//If C is pressed: Reset cam pos & zoom
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		mapView.reset(sf::FloatRect(-1000, -300, 2000, 900));

	//Do zoom
	if (ev.type == sf::Event::MouseWheelScrolled)
		mapView.zoom(1.0f + ev.mouseWheelScroll.delta * -0.1f);

	//Als knop gedrukt: ga alle knoppen langs & check
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		but.checkMouse(pos);
		window.setView(mapView);
	}
}