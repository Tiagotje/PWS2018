#pragma once
#include <SFML/Window.hpp>

class State {
public:
	virtual void calculate() = 0;
	virtual void draw() = 0;
	virtual void events(sf::Event ev) = 0;
};