#include <random>
#include <iostream>
#include <cstring>
#include "genetics.hpp"

//Soorten mutaties:
//
//PHYSIEK:
//Lengte mutaties 1/2 yes
//Hoek mutaties   1/3 yes
//Swap mutatie    1/10 yes
//Extra ledematen  1/10 yes
//Minder ledematen 1/10 yes
//Ander outputnummer spier 1/5 yes
//dominantie 1/8 per ding! yes
//
//MENTAAL:
//Verandering gewicht = 1/10 per gewicht!

std::uniform_real_distribution<> genetics::lengtemut(0, 2);
std::uniform_real_distribution<> genetics::hoekmut(0, 3);
std::uniform_real_distribution<> genetics::spiermut(0, 5);
std::uniform_real_distribution<> genetics::swapmut(0, 10);
std::uniform_real_distribution<> genetics::groeimut(0, 15);
std::uniform_real_distribution<> genetics::amputatiemut(0, 10);
std::uniform_real_distribution<> genetics::gewichtmut(0, 10);
std::uniform_real_distribution<> genetics::dominantiemut(0, 8);

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
		for (int j = 0; j < 10; j++)
			mutate(pop[i]);
		pop[i]->updatePos();
	}

	for (int i = 0; i < POPSIZE; i++) {
		pop[i]->limbs.clear();
		pop[i]->updateCreatureNodes();
	}

	std::cout << "dab" << std::endl;

	//testing
	pop[4] = new Creature(pop[2], pop[3]);

	std::cout << "dab" << std::endl;

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

float randD() {
	std::uniform_real_distribution<float> D(0, 1.0);
	return D(gen);
}

Node* getParent(Creature* c, Node* n) {
	for (int i = 0; i < c->limbs.size(); i++) {
		Node* p = c->limbs[i];
		for (int j = 0; j < p->nodes.size(); j++) {
			if (p->nodes[j] == n)
				return p;
		}
	}
	return NULL;
}

void mutate(Creature * c)
{
	while (genetics::lengtemut(gen) < 1) {
		if (c->limbs.size() == 0)
			break;
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
			if (c->limbs.size() - n->nodecount() < 3)
				continue;
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
			c->addNode(randL(), randA(), randN(), randD());
		}
		while (!added) {
			std::uniform_int_distribution<> node(0, c->limbs.size() - 1);
			Node * n = c->limbs[node(gen)];
			if(n->nodecount() < 7){
				n->addNode(randL(), randA(), randN(), randD());
				added = true;
			}
		}
	}

	while (genetics::swapmut(gen) < 1) {
		int tries = 0;
		while (tries < 10) {
			std::uniform_int_distribution<> node(0, c->limbs.size() - 1);
			Node * a = c->limbs[node(gen)];
			Node * b = c->limbs[node(gen)];
			if (a->contains(b) || b->contains(a) || a == b) {
				tries++;
				continue;
			}
			Node * pa = getParent(c, a);
			Node * pb = getParent(c, b);
			if (pa != NULL) {
				for (int i = 0; i < pa->nodes.size(); i++)
					if (pa->nodes[i] == a)
						pa->nodes[i] = b;
			}
			else {
				for (int i = 0; i < c->nodes.size(); i++)
					if (c->nodes[i] == a)
						c->nodes[i] = b;
			}

			if (pb != NULL) {
				for (int i = 0; i < pb->nodes.size(); i++)
					if (pb->nodes[i] == b)
						pb->nodes[i] = a;
			}
			else {
				for (int i = 0; i < c->nodes.size(); i++)
					if (c->nodes[i] == b)
						c->nodes[i] = a;
			}
			break;
		}
	}
}

void mutategenotype(Creature* p) {
	for (int i = 0; i < p->limbs.size(); i++)
		if (genetics::dominantiemut(gen) < 1)
			p->limbs[i]->dominance = randD();

	for (int i = 0; i < INPUTSIZE; i++)
		if (genetics::dominantiemut(gen) < 1)
			p->nn->dominance1[i] = randD();

	for (int i = 0; i < HIDDENSIZE; i++)
		if (genetics::dominantiemut(gen) < 1)
			p->nn->dominance2[i] = randD();
			
}

NN* fenonn(Creature* a, Creature* b) {
	NN* nn = new NN();
	NN* na = a->nn;
	NN* nb = b->nn;
	NN* source;
	for (int i = 0; i < INPUTSIZE; i++) {
		if (na->dominance1[i] > nb->dominance1[i])
			source = na;
		else
			source = nb;
		std::memcpy(nn->weights1[i], source->weights1[i], sizeof(float) * HIDDENSIZE);
		nn->dominance1[i] = source->dominance1[i];
	}
	for (int i = 0; i < HIDDENSIZE; i++) {
		if (na->dominance2[i] > nb->dominance2[i])
			source = na;
		else
			source = nb;
		std::memcpy(nn->weights2[i], source->weights2[i], sizeof(float) * OUTPUTSIZE);
		nn->dominance2[i] = source->dominance2[i];
	}

	return nn;
}


void crossingover(Creature * a, Creature * b) {
	std::uniform_int_distribution<> genA(0, a->nodes.size() - 1);
	std::uniform_int_distribution<> genB(0, b->nodes.size() - 1);
	Node * na = a->limbs[genA(gen)];
	Node * nb = b->limbs[genB(gen)];
	Node * pna = getParent(a, na);
	Node * pnb = getParent(b, nb);
	if (pna == NULL)
		for (int i = 0; i < a->nodes.size(); i++)
			if (a->nodes[i] == na)
				a->nodes[i] = nb;
	if (pnb == NULL)
		for (int i = 0; i < b->nodes.size(); i++)
			if (b->nodes[i] == nb)
				b->nodes[i] = na;
	if (pna != NULL)
		for (int i = 0; i < pna->nodes.size(); i++)
			if (pna->nodes[i] == na)
				pna->nodes[i] = nb;
	if (pnb != NULL)
		for (int i = 0; i < pnb->nodes.size(); i++)
			if (pnb->nodes[i] == nb)
				pnb->nodes[i] = na;
	a->limbs.clear();
	b->limbs.clear();
	a->updateCreatureNodes();
	b->updateCreatureNodes();
}

//a + b -> c
//1. doe random geschud 5x
//2. grootste dominance wint
//functie returnt de 'fenotype' creature en de OG creature kopieert gwn hiervan
Creature feno(Creature* p1, Creature* p2) {
	Creature a = Creature(p1);
	Creature b = Creature(p2);
	a.updateCreatureNodes();
	b.updateCreatureNodes();
	for (int i = 0; i < 5; i++)
		crossingover(&a, &b);
	float domA = 0;
	float domB = 0;
	for (int i = 0; i < a.limbs.size(); i++)
		domA += a.limbs[i]->dominance;
	for (int i = 0; i < b.limbs.size(); i++)
		domB += b.limbs[i]->dominance;
	return (domA > domB ? a : b);
}