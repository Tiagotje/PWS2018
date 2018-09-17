#pragma once

#include "state.hpp"
#include "button.hpp"
#include <SFML/Graphics.hpp>

class MenuState : public State {
public:
	MenuState();
	void draw();
	void events(sf::Event);
	void calculate();
private:
	sf::Text title;
	Button but;
};