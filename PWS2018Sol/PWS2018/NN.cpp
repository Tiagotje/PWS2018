#include "NN.hpp"
#include "creature.hpp"
#include <math.h>
#include <iostream>
#include <random>

NN::NN(Creature * c)
{
	creature = c;
}


void NN::calculate(float a, float b, float c, float d, float e, float f, float g)
{
	//eten  richting & afstand
	input[0] = a;
	input[1] = fmin(1, b / 50);
	
	//benen
	input[2] = c;
	input[3] = d;
	input[4] = e;
	input[5] = f;

	//honger
	input[6] = g;

	//proprioceptie
	for (int i = 0; i < OUTPUTSIZE; i++)
		input[7 + i] = output[i];

	//HIDDEN LAYER
	for (int i = 0; i < HIDDENSIZE; i++)
		hidden[i] = 0;

	for (int i = 0; i < HIDDENSIZE; i++)
		for (int j = 0; j < INPUTSIZE; j++)
			hidden[i] += weights1[j][i] * input[i];

	for (int i = 0; i < HIDDENSIZE; i++)
		hidden[i] = sigmoid(hidden[i]);

	//OUTPUTLAYERR
	for (int i = 0; i < OUTPUTSIZE; i++)
		output[i] = 0;

	for (int i = 0; i < OUTPUTSIZE; i++)
		for (int j = 0; j < HIDDENSIZE; j++)
			output[i] += weights2[j][i] * hidden[i];

	for (int i = 0; i < OUTPUTSIZE; i++)
		output[i] = sigmoid(output[i]);

}

void NN::initweights() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	std::cout << dis(gen) << std::endl;
}

//ipv sigmoid doen we tanh, is sneller  (1/1+abs(x) kan ook?)
inline float NN::sigmoid(float v) 
{
	return tanh(v);
}