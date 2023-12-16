#include <stdint.h>
#include <RNG.h>
#include <stdlib.h>

// Function to initialize the random number generator with parameters
void initRandomGenerator(int seed) {
    srand(seed);
}


// Function to generate a random unsigned integer
int getRandomInt(int max){
	int ret;
	int p = 1;
	while (p < max){
		p *= 2;
	}
	do{
		ret = rand()%p;
	}while(ret<0 || ret>=max);
	return ret;
}

// Function to generate a random float in the range [0, 1)
double getRandomDouble() {
    return rand() / (double)RAND_MAX;
}


