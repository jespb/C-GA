/* 
* By using this file, you are agreeing to this product's EULA
*
* This product can be obtained in https://github.com/jespb/C-GA
*
* Copyright ©2023 J. E. Batista
*/


#ifndef _GENETICOPERATORS_H
#define _GENETICOPERATORS_H


Individual roulette(RNG *rng, const Individual population[], int p_size);

void getElite(const Individual population[], int p_size, Individual elite[], int e_size);
void getOffspring(RNG *rng, const Individual population[], int p_size, Individual offspring[] );

void STXO(RNG *rng, Individual *parent1, Individual *parent2 );
void STMUT(RNG *rng, Individual *parent);


#endif


