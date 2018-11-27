//NeueralNetwork.hpp

//INPUT:
//0 richting eten (-1L, +1R)
//1 afstand food  max(1, afstand/50)
//2,3,4,5  raakt been 1 - 4 grond aan? 1 : 0
//6 honger 0 = start energie, minder energie -> -1
//7 ..  de N outputs van vorige keer

#pragma once

#include "creature.hpp"
#include <iostream>

#define INPUTSIZE 14
#define HIDDENSIZE 10
#define OUTPUTSIZE 10

const int outputsize = OUTPUTSIZE;

class Creature;

class NN {
public:
	NN() {};
	//~NN() { std::cout << "~nn" << std::endl; };
	NN(Creature * c);
	NN(NN * nn);
	float weights1[INPUTSIZE][HIDDENSIZE];
	float weights2[HIDDENSIZE][OUTPUTSIZE];
	float dominance1[INPUTSIZE];
	float dominance2[HIDDENSIZE];
	float input[INPUTSIZE];
	float hidden[HIDDENSIZE];
	float output[OUTPUTSIZE];
	void calculate(float, float, float);
	void initweights();
private:
	inline float sigmoid(float v);
	Creature * creature;
};