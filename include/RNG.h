/* 
* By using this file, you are agreeing to this product's EULA
*
* This product can be obtained in https://github.com/jespb/C-GA
*
* Copyright ©2023 J. E. Batista
*/


#ifndef _RNG_H
#define _RNG_H

// Define a struct for the random number generator
typedef struct {
    int state;
} RNG;

void initRandomGenerator(RNG* rng, int seed);
int getRandomInt(RNG* rng, int max);
double getRandomDouble(RNG* rng);