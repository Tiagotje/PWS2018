#include "NN.hpp"
#include "creature.hpp"
#include <math.h>
#include <iostream>
#include <random>

NN::NN(Creature * c)
{
	creature = c;
}

int times = 0;

void NN::calculate(float a, float b, float c, float d, float e, float f, float g)
{
	//eten  richting & afstand
	input[0] = a;
	input[1] = (float) fmin(1, b / 50);
	
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


	//print output 0 tm 14 (zie patroon!)
	if (times < 15)
	{
		std::cout << "Output: ";
		for (int i = 0; i < OUTPUTSIZE; i++)
			std::cout << output[i] << (i+1 == OUTPUTSIZE ? "" : ", ");
		std::cout << std::endl;
	}

	times++;
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