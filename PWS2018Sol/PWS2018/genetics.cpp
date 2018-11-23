#include <random>
#include <iostream>
#include "genetics.hpp"

//Soorten mutaties:
//
//PHYSIEK:
//Lengte mutaties 1/2
//Hoek mutaties   1/3
//Swap mutatie    1/10
//Extra ledematen  1/10
//Minder ledematen 1/10
//Ander outputnummer spier 1/5
//dominantie 1/8 per ding!
//
//MENTAAL:
//Verandering gewicht = 1/10 per gewicht!
std::uniform_real_distribution<> genetics::lengtemut(0, 2);
std::uniform_real_distribution<> genetics::hoekmut(0, 3);
std::uniform_real_distribution<> genetics::spiermut(0, 5);
std::uniform_real_distribution<> genetics::swapmut(0, 10);
std::uniform_real_distribution<> genetics::groeimut(0, 10);
std::uniform_real_distribution<> genetics::amputatiemut(0, 10);
std::uniform_real_distribution<> genetics::gewichtmut(0, 10);

std::random_device rd;
std::mt19937 gen(rd());

std::vector<Creature*> genPopulation()
{
	//popsize = 50?
	std::vector<Creature*> pop;

	for (int i = 0; i < POPSIZE; i++) {
		pop.push_back(new Creature());
	}

	for (int i = 0; i < POPSIZE; i++) {
		for (int j = 0; j < 20; j++)
			mutate(pop[i]);
		pop[i]->updatePos();
	}
	return pop;
}

double clamp(double min, double x, double max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}


float randL() {
	std::normal_distribution<double> L(1.6, 0.5);
	return (float)clamp(2.0, pow(L(gen), 2), 6.0);
}

float randA() {
	std::normal_distribution<double> A(0.0, 75.0);
	return A(gen) / 360 * 2 * b2_pi;
}

int randN() {
	std::uniform_int_distribution<> N(0, 9);
	return N(gen);
}


void mutate(Creature * c)
{
	while (genetics::lengtemut(gen) < 1) {
		std::uniform_int_distribution<> limb(0, c->limbs.size()-1);
		std::normal_distribution<double> factor(1.0, 0.25);
		int L = limb(gen);
		double f = factor(gen);
		if (f < 1.0)
			f = 1/(2 - f);
		c->limbs[L]->limb.length = clamp(2.0, f*c->limbs[L]->limb.length, 6.0);
	}

	while (genetics::hoekmut(gen) < 1) {
		std::uniform_int_distribution<> limb(0, c->limbs.size() - 1);
		std::normal_distribution<double> factor(0.0, 1.0);
		int L = limb(gen);
		double f = factor(gen);
		double val = f + c->limbs[L]->angle;
		c->limbs[L]->angle = val;
		c->limbs[L]->limb.sAngle = val;
	}

	while (genetics::spiermut(gen) < 1) {
		std::uniform_int_distribution<> spier(0, c->limbs.size() - 1);
		std::uniform_int_distribution<> neuron(0, outputsize - 1);
		c->limbs[spier(gen)]->neuron = neuron(gen);
	}

	while (genetics::amputatiemut(gen) < 1) {
		bool removed = false;
		if (c->limbs.size() < 3)
			break;
		while (!removed) {
			std::uniform_int_distribution<> node(0, c->limbs.size() - 1);
			Node * n = c->limbs[node(gen)];
			std::uniform_int_distribution<> subnodes(0, n->nodecount());
			if (subnodes(gen) >= 1)
				continue;
			c->deleteNodes(n);
			removed = true;
		}
	}

	while (genetics::groeimut(gen) < 1) {
		bool added = false;
		if (c->limbs.size() > 12)
			break;
		std::uniform_int_distribution<> newlimb(0, c->limbs.size());
		if(newlimb(gen) < 3){
			c->addNode(randL(), randA(), randN());
		}
		while (!added) {
			std::uniform_int_distribution<> node(0, c->limbs.size() - 1);
			Node * n = c->limbs[node(gen)];
			if(n->nodecount() < 7){
				n->addNode(randL(), randA(), randN());
				added = true;
			}
		}
	}
}