#pragma once

#include "creature.hpp"
#include <random>

#define POPSIZE 50

std::vector<Creature*> genPopulation();
void mutate(Creature * c);


namespace genetics {
	extern std::uniform_real_distribution<> lengtemut;
	extern std::uniform_real_distribution<> hoekmut;
	extern std::uniform_real_distribution<> spiermut;
	extern std::uniform_real_distribution<> swapmut;
	extern std::uniform_real_distribution<> groeimut;
	extern std::uniform_real_distribution<> amputatiemut;
	extern std::uniform_real_distribution<> gewichtmut;
}