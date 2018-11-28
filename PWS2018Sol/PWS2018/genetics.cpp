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

std::uniform_real_distribution<> genetics::lengtemut(0,5);
std::uniform_real_distribution<> genetics::hoekmut(0,5);
std::uniform_real_distribution<> genetics::spiermut(0,10);
std::uniform_real_distribution<> genetics::swapmut(0,20);
std::uniform_real_distribution<> genetics::groeimut(0,10);
std::uniform_real_distribution<> genetics::amputatiemut(0,10);
std::uniform_real_distribution<> genetics::gewichtmut(0,25);
std::uniform_real_distribution<> genetics::dominantiemut(0,10);

std::random_device rd;
std::mt19937 gen(rd());

std::vector<Creature*> OLDgenPopulation()
{
	//gen 2 creatures
	std::vector<Creature*> parpop;

	for (int i = 0; i < POPSIZE; i++) {
		parpop.push_back(new Creature());
		parpop[i]->nn->initweights();
	}

	for (int i = 0; i < POPSIZE; i++) {
		for (int j = 0; j < 10; j++)
			mutate(parpop[i]);
		parpop[i]->updatePos();
	}

	for (int i = 0; i < POPSIZE; i++) {
		parpop[i]->limbs.clear();
		parpop[i]->updateCreatureNodes();
	}

	std::vector<Creature*> pop;

	std::uniform_int_distribution<> chooseparents(0, POPSIZE - 1);
	for (int i = 0; i < POPSIZE; i++) {
		int a = chooseparents(gen);
		int b = chooseparents(gen);
		if (a == b)
			b = (b + 1) % POPSIZE;
		pop.push_back(new Creature(parpop[a], parpop[b]));
	}
	
	return pop;
}

std::vector<Creature*> genPopulation()
{
	//gen 2 creatures
	Creature * p1 = new Creature();
	Creature * p2 = new Creature();

	p1->nn->initweights();
	p2->nn = p1->nn;

	for (int i = 0; i < 4; i++) {
		mutate(p1);
		mutate(p2);
	}

	Creature * p = new Creature(p1, p2);

	std::vector<Creature*> pop;

	for (int i = 0; i < POPSIZE; i++) {
		pop.push_back(new Creature(&gengeno(p), &gengeno(p)));
	}

	return pop;
}

std::vector<int> roulette(std::vector<Creature *> old, float * fitness) {
	std::vector<int> ret;

	std::vector<int> list;
	for (int i = 0; i < POPSIZE; i++) {
		int v = (int)fitness[i];
		for (int j = 0; j < v; j++)
			list.push_back(i);
	}

	std::uniform_int_distribution<> dist1(0, list.size() - 1);
	int i = list[dist1(gen)];
	ret.push_back(i);

	list.erase(std::remove(list.begin(), list.end(), i), list.end());

	if (list.size() == 0) {
		ret.push_back(0);
	}
	else {
		std::uniform_int_distribution<> dist2(0, list.size() - 1);
		i = list[dist1(gen)];
		ret.push_back(i);
	}

	return ret;
}

std::vector<Creature *> genNewPop(std::vector<Creature *> old, float * fitness) {
	std::vector<Creature *> newpop;
	float sum = 0.0f;
	for (int i = 0; i < POPSIZE; i++)
		sum += fitness[i];

	for (int i = 0; i < POPSIZE; i++) {
		std::vector<int> ids = roulette(old, fitness);
		Creature a = gengeno(old[ids[0]]);
		Creature b = gengeno(old[ids[1]]);
		newpop.push_back(new Creature(&a, &b));
	}
	return newpop;
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
	std::uniform_real_distribution<float> L(2.0, 5.0);
	return L(gen);
}

float randA() {
	std::uniform_real_distribution<float> A(-b2_pi, b2_pi);
	return A(gen);
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
		int L = limb(gen);
		c->limbs[L]->limb.length = randL();
	}

	while (genetics::hoekmut(gen) < 1) {
		std::uniform_int_distribution<> limb(0, c->limbs.size() - 1);
		int L = limb(gen);
		float val = randA();
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
		break; //onrealistisch
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

void mutateNN(NN* nn) {
	std::uniform_real_distribution<float> weight(-1.0, 1.0);

	for (int i = 0; i < INPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++)
			if (genetics::gewichtmut(gen) < 1)
				nn->weights1[i][j] = weight(gen);

	for (int i = 0; i < HIDDENSIZE; i++)
		for (int j = 0; j < OUTPUTSIZE; j++)
			if (genetics::gewichtmut(gen) < 1)
				nn->weights1[i][j] = weight(gen);
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

//NEW CROSSING OVER ALGO VOOR BIJ GENGENO
Creature * NEWcrossingover(Creature * a, Creature * b) {
	Creature * c = new Creature();
	c->nodes.clear();
	for (int i = 0; i < c->limbs.size(); i++)
		delete c->limbs[i];
	c->limbs.clear();
	std::uniform_int_distribution<> bg(0, 1);
	int aNS = a->nodes.size();
	int bNS = b->nodes.size();
	int min = (aNS < bNS ? aNS : bNS);
	int max = (aNS > bNS ? aNS : bNS);
	for (int i = 0; i < min; i++)
		c->nodes.push_back((bg(gen) == 0 ? a->nodes[i] : b->nodes[i]));
	Creature * big = (aNS > bNS ? a : b);
	for (int i = min; i < max; i++)
		if (bg(gen) == 0)
			c->nodes.push_back(big->nodes[i]);

	return c;
}
//a + b -> c
//1. doe random geschud 5x
//2. grootste dominance wint
//functie returnt de 'fenotype' creature en de OG creature kopieert gwn hiervan
Creature feno(Creature* p1, Creature* p2) {
	Creature a = Creature(p1);
	Creature b = Creature(p2);
	a.limbs.clear();
	b.limbs.clear();
	a.updateCreatureNodes();
	b.updateCreatureNodes();
	//crossingover(&a, &b);
	a.updatePos();
	b.updatePos();
	float domA = 0;
	float domB = 0;
	for (int i = 0; i < a.limbs.size(); i++)
		domA += a.limbs[i]->dominance;
	for (int i = 0; i < b.limbs.size(); i++)
		domB += b.limbs[i]->dominance;
	domA = domA / a.limbs.size();
	domB = domB / b.limbs.size();
	return (domA > domB ? a : b);
}


void swapfloat(float &a, float &b) {
	float c = a;
	a = b;
	b = c;
}

void crossNN(NN* a, NN* b) {
	std::uniform_int_distribution<> dist(0, 9);
	bool swap = true;
	//w1
	for (int i = 0; i < INPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++) {
			if (swap)
				swapfloat(a->weights1[i][j], b->weights1[i][j]);
			if (dist(gen) == 0)
				swap = !swap;
		}

	//w2
	for (int i = 0; i < HIDDENSIZE; i++)
		for (int j = 0; j < OUTPUTSIZE; j++) {
			if (swap)
				swapfloat(a->weights2[i][j], b->weights2[i][j]);
			if (dist(gen) == 0)
				swap = !swap;
		}
	
	//d1
	for (int i = 0; i < INPUTSIZE; i++)
		if (swap)
			swapfloat(a->dominance1[i], b->dominance1[i]);
		if (dist(gen) == 0)
			swap = !swap;

	//d2
	for (int i = 0; i < HIDDENSIZE; i++)
		if (swap)
			swapfloat(a->dominance2[i], b->dominance2[i]);
		if (dist(gen) == 0)
			swap = !swap;
	
}

Creature gengeno(Creature * c) {
	Creature a = Creature(c->parents[0]);
	Creature b = Creature(c->parents[1]);
	a.limbs.clear();
	b.limbs.clear();
	a.updateCreatureNodes();
	b.updateCreatureNodes();
	NEWcrossingover(&a, &b);
	a.updatePos();
	NN* na = c->parents[0]->nn;
	NN* nb = c->parents[1]->nn;
	crossNN(na, nb);
	delete a.nn;
	a.nn = na;
	mutate(&a);
	mutategenotype(&a);
	return a;
}