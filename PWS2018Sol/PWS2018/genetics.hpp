#pragma once

#include "creature.hpp"
#include <random>
#include <vector>

#define POPSIZE 25

std::vector<Creature*> genPopulation();
void mutate(Creature * c);
void mutategenotype(Creature * p);
NN* fenonn(Creature* a, Creature* b);
Creature feno(Creature * a, Creature * b);
Creature gengeno(Creature * c);
std::vector<Creature *> genNewPop(std::vector<Creature *> old, float * fitness);


namespace genetics {
	extern std::uniform_real_distribution<> lengtemut;
	extern std::uniform_real_distribution<> hoekmut;
	extern std::uniform_real_distribution<> spiermut;
	extern std::uniform_real_distribution<> swapmut;
	extern std::uniform_real_distribution<> groeimut;
	extern std::uniform_real_distribution<> amputatiemut;
	extern std::uniform_real_distribution<> gewichtmut;
	extern std::uniform_real_distribution<> dominantiemut;
}