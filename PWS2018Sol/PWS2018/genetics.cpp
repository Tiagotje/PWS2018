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
//
//MENTAAL:
//Verandering gewicht  gemiddeld 27 gewichten per keer = 1/10 per gewicht!
std::uniform_real_distribution<> genetics::lengtemut(0, 2);
std::uniform_real_distribution<> genetics::hoekmut(0, 3);
std::uniform_real_distribution<> genetics::spiermut(0, 5);
std::uniform_real_distribution<> genetics::swapmut(0, 10);
std::uniform_real_distribution<> genetics::groeimut(0, 10);
std::uniform_real_distribution<> genetics::amputatiemut(0, 10);
std::uniform_real_distribution<> genetics::gewichtmut(0, 10);

std::vector<Creature*> genPopulation()
{


	//popsize = 50?
	std::vector<Creature*> pop;

	//standard creature
	pop.push_back(new Creature());
	pop[0]->addNode(4, 0);
	pop[0]->addNode(4, (-0.5*b2_pi));
	pop[0]->addNode(4, b2_pi);
	pop[0]->nodes[0].addNode(4, (-0.5*b2_pi));
	pop[0]->nodes[2].addNode(4, (0.5*b2_pi));

	for (int i = 1; i < 50; i++) {
		pop.push_back(new Creature());
		*pop[i] = *pop[0];
		pop[i]->updateCreature();
	}

	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 20; j++)
			mutate(pop[i]);
		pop[i]->updatePos();
	}

	std::cout << pop[1]->nodes[0].limb.length << std::endl;

	return pop;
}


void mutate(Creature * c)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	//bv: genetics::lengtemut(gen);

	while (genetics::lengtemut(gen) < 1) {
		std::uniform_int_distribution<> limb(0, c->limbs.size()-1);
		std::normal_distribution<double> factor(1.0, 0.25);
		int l = limb(gen);
		double f = factor(gen);
		if (f < 1.0)
			f = 1/(2 - f);
		c->limbs[l]->limb.length *= f;
	}

	while (genetics::hoekmut(gen) < 1) {
		std::uniform_int_distribution<> limb(0, c->limbs.size()-1);
		std::normal_distribution<double> factor(0.0, 1.0);
		int l = limb(gen);
		double f = factor(gen);
		if (f < 1.0)
			f = 1 / (2 - f);
		c->limbs[l]->angle += f;
		c->limbs[l]->limb.sAngle += f;
	}

}

//Fenotype vs Genotype
//Fenotype gewichten = grootste abs| |
// 