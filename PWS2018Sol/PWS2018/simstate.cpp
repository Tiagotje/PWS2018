#include "simstate.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "main.hpp"
#include "terrain.hpp"
#include "fonts.hpp"
#include "button.hpp"
#include "fonts.hpp"

void dab();

static int lengte = 20;

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
	but.setText("PENIS", 25);
	but.setSize(sf::FloatRect(150, 5, 100, 35));
	but.setFunc(dab);
	but.setColor(sf::Color::Black);
	but.setTextPos(10, 1);
}

//Nu nog leeg...
void SimState::calculate() {  }

void dab()
{
	std::cout << "Mijn penis is zo kort :( :" << std::endl;
	std::cout << "8";
	for (int i = lengte; i > 0; i--)
		std::cout << "=";
	std::cout << "D" << std::endl;
	lengte++;
}


void SimState::draw()
{
	window.clear(sf::Color(75, 100, 255));		// (115,170,255) = RGB lichtblauw

	//tekent map
	window.setView(mapView);
	window.draw(ground);

	//Tekent onderkant
	sf::RectangleShape rect(sf::Vector2f(1600, 90));
	rect.setFillColor(sf::Color::Red);
	window.setView(lowerView);
	window.draw(rect);

	//Tekent bovenkant
	rect.setSize(sf::Vector2f(1600, 45));
	window.setView(upperView);
	window.draw(rect);
	but.draw();
}


void SimState::events()
{
	//Luister naar input
	sf::Event ev;
	while (window.pollEvent(ev))
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
			mapView.reset(sf::FloatRect(-1000, 0, 2000, 1000));

		//Do zoom
		if (ev.type == sf::Event::MouseWheelScrolled)
			mapView.zoom(1.0f + ev.mouseWheelScroll.delta * -0.1f);

		//Als knop gedrukt: ga alle knoppen langs & check
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			but.checkMouse(pos);
		}
	}
}