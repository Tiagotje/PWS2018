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
	BallObject bal = BallObject(20, 0);
	RodObject rod = RodObject(4, 0.5, b2Vec2(-2, 0), 0);
	RodObject rod2 = RodObject(4, 0.5, b2Vec2(2, 0), 0);
	Button but;
};