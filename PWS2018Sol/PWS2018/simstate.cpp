#include "simstate.hpp"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "main.hpp"
#include "terrain.hpp"
#include "util.hpp"
#include "button.hpp"

void dab();

SimState::SimState()
{
	Util::initFonts();
	//reset geeft aan welk deel van de view op het scherm moet komen
	//Viewport geeft in fracties aan waar op het scherm deze getekend moet worden
	upperView.reset(sf::FloatRect(0, 0, 1600, 45));
	upperView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 0.05f));
	mapView.reset(sf::FloatRect(-1000, 0, 2000, 1000));
	mapView.setViewport(sf::FloatRect(0.0f, 0.05f, 1.0f, 0.85f));
	lowerView.reset(sf::FloatRect(0, 0, 1600, 90));
	lowerView.setViewport(sf::FloatRect(0.0f, 0.9f, 1.0f, 0.1f));

	//Genereert de grond; 
	ground = TerrainGen();

	//De testknop
	but.setView(upperView);
	but.setFont(Util::dafont);
	but.setText("D E R P", 25);
	but.setSize(sf::FloatRect(150, 5, 100, 35));
	but.setFunc(dab);
	but.setColor(sf::Color::Black);
	but.setTextPos(10, 1);

	Phys::revolute(rod.body, rod2.body, b2Vec2(0, 0));

}

//Nu nog leeg...
void SimState::calculate() {  }

void dab()
{
	std::cout << "Dit doet niets!!" << std::endl;
}


void SimState::draw()
{
	window.clear(sf::Color(75, 100, 255));

	//tekent map
	window.setView(mapView);
	window.draw(ground);
	bal.draw();
	rod.draw();
	rod2.draw();

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
		mapView.reset(sf::FloatRect(-1000, 200, 2000, 1000));

	//FORCES
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		bal.force(-4000, 0);

	//FORCES
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		bal.force(4000, 0);

	//FORCES
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		bal.force(0, 12000);

	//FORCES
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		Phys::joint->SetMotorSpeed(10.0);
		Phys::joint->EnableMotor(true);
		Phys::joint->SetMaxMotorTorque(100);
	}

	//Do zoom
	if (ev.type == sf::Event::MouseWheelScrolled)
		mapView.zoom(1.0f + ev.mouseWheelScroll.delta * -0.1f);

	//Als knop gedrukt: ga alle knoppen langs & check
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		but.checkMouse(pos);
		window.setView(mapView);
		sf::Vector2f worldPos = window.mapPixelToCoords(pos);
		std::cout << "X: " << worldPos.x << " Y: " << worldPos.y << std::endl;
	}
}