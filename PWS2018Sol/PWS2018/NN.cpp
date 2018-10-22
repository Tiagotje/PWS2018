#include "NN.hpp"
#include <math.h>

void NN::calculate()
{
	//eten  richting & afstand
	input[0] = 0.0; 
	input[1] = fmin(1, 0.0 / 50);
	
	//benen
	input[2] = 0.0;
	input[3] = 0.0;
	input[4] = 0.0;
	input[5] = 0.0;

	//honger
	input[6] = 0.0;

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

//ipv sigmoid doen we tanh, is sneller  (1/1+abs(x) kan ook?)
inline float NN::sigmoid(float v) 
{
	return tanh(v);
}