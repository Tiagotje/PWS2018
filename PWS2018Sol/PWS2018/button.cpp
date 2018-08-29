#include "button.hpp"

#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "main.hpp"
#include "util.hpp"

//lege constructor
Button::Button() { }

//Alles in 1 keer, idk of we dat willen
Button::Button(sf::View v, sf::FloatRect fr, sf::Color c,
	std::string s, int fontsize, void(*f)())
{
	setSize(fr);
	setView(v);
	setColor(c);
	setFunc(f);
	setFont(Util::dafont);
	setText("oi", fontsize);
}

void Button::setFont(sf::Font f)
{
	font = f;
}

void Button::setSize(sf::FloatRect fr)
{
	rect.setSize(sf::Vector2f(fr.width, fr.height));
	rect.setPosition(sf::Vector2f(fr.left, fr.top));
}

//plaats van text relatief aan plaats knop
void Button::setTextPos(int x, int y)
{
	buttonText.setPosition(rect.getPosition().x + x, rect.getPosition().y + y);
}

//Doet func als muis erin zit. Wegens views moeten coords eerst worden omgerekent
void Button::checkMouse(sf::Vector2i vec)
{
	window.mapPixelToCoords(vec, view);
	if (rect.getGlobalBounds().contains((float)vec.x, (float)vec.y))
		func();
}

void Button::setView(sf::View v)
{
	view = v;
}

void Button::setFunc(void(*f)())
{
	func = f;
}

//Verander text & formaat & kleur
void Button::setText(std::string s, int fontsize)
{
	buttonText.setString(s);
	buttonText.setCharacterSize(fontsize);
	buttonText.setFillColor(sf::Color::White);
}

//Achtergrondkleur van knop
void Button::setColor(sf::Color c)
{
	rect.setFillColor(c);
}

//Tekent
void Button::draw()
{
	window.setView(view);
	window.draw(rect);
	buttonText.setFont(font); 	//Hij crasht zonder dit???
	window.draw(buttonText);
}