/* 
* By using this file, you are agreeing to this product's EULA
*
* This product can be obtained in https://github.com/jespb/C-GA
*
* Copyright ©2023 J. E. Batista
*/

#include <stdint.h>

#ifndef _RNG_H
#define _RNG_H

void initRandomGenerator(int seed);
int getRandomInt(int max);
double getRandomDouble();


#endif