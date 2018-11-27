#include "NN.hpp"
#include "creature.hpp"
#include <math.h>
#include <iostream>
#include <random>
#include <cstring>


NN::NN(Creature * c)
{
	creature = c;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	for (int i = 0; i < INPUTSIZE; i++) 
		dominance1[i] = dis(gen);
	for (int i = 0; i < HIDDENSIZE; i++) 
		dominance2[i] = dis(gen);

	initweights();
}

NN::NN(NN * nn)
{
	//just copy data
	std::memcpy(weights1[0], nn->weights1[0], sizeof(float)*INPUTSIZE*HIDDENSIZE);
	std::memcpy(weights2[0], nn->weights2[0], sizeof(float)*HIDDENSIZE*OUTPUTSIZE);
	std::memcpy(dominance1, nn->dominance1, sizeof(float)*INPUTSIZE);
	std::memcpy(dominance2, nn->dominance2, sizeof(float)*HIDDENSIZE);
}


int times = 0;

void NN::calculate(float richting, float afstand, float honger)
{
	input[0] = 1.0f;

	//eten  richting & afstand
	input[1] = richting;
	input[2] = sigmoid(afstand/10);

	//honger
	input[3] = sigmoid((honger-1000)/1000);

	//proprioceptie
	for (int i = 0; i < OUTPUTSIZE; i++)
		input[3 + i] = output[i];

	//HIDDEN LAYER
	for (int i = 0; i < HIDDENSIZE; i++)
		hidden[i] = 0.0;

	for (int i = 0; i < HIDDENSIZE; i++)
		for (int j = 0; j < INPUTSIZE; j++)
		{	
			hidden[i] += weights1[j][i]*input[j];
		}

	for (int i = 0; i < HIDDENSIZE; i++)
		hidden[i] = sigmoid(hidden[i]);

	//OUTPUTLAYERR
	for (int i = 0; i < OUTPUTSIZE; i++)
		output[i] = 0.0;

	for (int i = 0; i < OUTPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++)
			output[i] += weights2[j][i] * hidden[i];

	for (int i = 0; i < OUTPUTSIZE; i++)
		output[i] = sigmoid(output[i]);
}

void NN::initweights() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.5, 1.5);

	for (int i = 0; i < HIDDENSIZE; i++)
		for (int j = 0; j < INPUTSIZE; j++)
			weights1[j][i] = (float) dis(gen);

	for (int i = 0; i < OUTPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++)
			weights2[j][i] = (float) dis(gen);

	for (int i = 0; i < OUTPUTSIZE; i++)
		output[i] = 0.0f;

}

//ipv sigmoid doen we tanh, is sneller  (1/1+abs(x) kan ook?)
inline float NN::sigmoid(float v) 
{
	return tanh(v);
}