#pragma once

#include "state.hpp"
#include "button.hpp"
#include "physics.hpp"
#include "creature.hpp"
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
	Limb limb = Limb(b2Vec2(0, 0), b2Vec2(10,-20));
	Button but;
};