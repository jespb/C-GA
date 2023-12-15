#include <stdint.h>
#include <RNG.h>

// Function to initialize the random number generator with a seed
void initRandomGenerator(RNG* rng, int seed) {
    rng->state = seed;
}

// Function to generate a random unsigned integer
int getRandomInt(RNG *rng, int max){
    // XOR-Shift algorithm
    rng->state ^= (rng->state << 13);
    rng->state ^= (rng->state >> 17);
    rng->state ^= (rng->state << 5);
    return rng->state % max;
}

// Function to generate a random float in the range [0, 1)
double getRandomDouble(RNG* rng) {
    // Convert the random integer to a float in the range [0, 1)
    return getRandomInt(rng, UINT32_MAX) / (double)UINT32_MAX;
}