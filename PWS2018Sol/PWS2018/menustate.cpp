#include "menustate.hpp"
#include "util.hpp"
#include "main.hpp"
#include "simstate.hpp"

#include <iostream>

void start();

MenuState::MenuState()
{
	Util::initFonts();
	window.setView(window.getDefaultView());

	title.setFont(Util::dafont);
	title.setString("ONS SUPER STOERE PWS");
	title.setCharacterSize(50);
	std::cout << title.getLocalBounds().width << std::endl;
	title.setPosition(sf::Vector2f((1600 - title.getLocalBounds().width) / 2, 100));
	but.setView(window.getDefaultView());
	but.setFont(Util::dafont);
	but.setText("YooOOOOoo\nKlik HIER", 25);
	but.setSize(sf::FloatRect(600, 500, 400, 100));
	but.setFunc(start);
	but.setColor(sf::Color::Red);
	but.setTextPos(10, 0);

}

void MenuState::draw()
{
	window.clear(sf::Color(119, 167, 224));
	window.draw(title);
	but.draw();
}

void MenuState::events(sf::Event ev)
{
	switch (ev.type) {
	
	case sf::Event::MouseButtonPressed:
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		but.checkMouse(pos);
	}
};

void MenuState::calculate() {};

void start() {
	delete state;
	state = new SimState();
}