#pragma once

#include "state.hpp"
#include "button.hpp"
#include "physics.hpp"
#include <SFML/Graphics.hpp>

class SimState : public State {
public:
	SimState();
	void calculate();
	void draw();
	void events(sf::Event);

private:
	sf::View mapView, lowerView, upperView;
	sf::VertexArray ground;
	BallObject bal = BallObject(0, 0);
	Button but;
};