#pragma once

#include "state.hpp"
#include "button.hpp"
#include "physics.hpp"
#include "creature.hpp"
#include "genetics.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class SimState : public State {
public:
	SimState();
	void calculate();
	void draw();
	void events(sf::Event);
	//moet pointer zijn, omdat daadwerkelijke inhoud van n vector kan bewegen in memory
	std::vector<Creature*> pop;
	float fitness[POPSIZE];
	Creature * active;
	void nextCreature();
private:
	int creatureID = 0;
	sf::View mapView, lowerView, upperView;
	sf::VertexArray ground;
	Button nextbut;
	Button pauzebut;
	Button slowbut;
};