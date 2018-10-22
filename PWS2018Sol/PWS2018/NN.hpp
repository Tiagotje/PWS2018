//NeueralNetwork.hpp

//INPUT:
//0 richting eten (-1L, +1R)
//1 afstand food  max(1, afstand/50)
//2,3,4,5  raakt been 1 - 4 grond aan? 1 : 0
//6 honger 0 = start energie, minder energie -> -1
//7 ..  de N outputs van vorige keer

#define INPUTSIZE 12
#define HIDDENSIZE 20
#define OUTPUTSIZE 5

class NN {
public:
	float weights1[INPUTSIZE][HIDDENSIZE];
	float weights2[HIDDENSIZE][OUTPUTSIZE];
	float input[INPUTSIZE];
	float hidden[HIDDENSIZE];
	float output[OUTPUTSIZE];
	void calculate();
private:
	inline float sigmoid(float v);
};