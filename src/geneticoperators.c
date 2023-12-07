/*
 * By using this file, you are agreeing to this product's EULA
 *
 * This product can be obtained in https://github.com/jespb/C-GA
 *
 * Copyright ©2023 J. E. Batista
 */

#include <stdio.h>
#include <stdlib.h>

#include <geneticoperators.h>
#include <individual.h>


Individual roulette(RNG rng, const Individual population[], int p_size);

void getElite(const Individual population[], int p_size, Individual elite[], int e_size);
void getOffspring(RNG rng, const Individual population[], int p_size, Individual offspring[] );

void STXO(RNG rng, Individual parent1, Individual parent2 );
void STMUT(RNG rng, Individual parent);




Individual roulette(RNG rng, const Individual population[], int p_size){
  double max = 0, select;
  int i;
  for(i = 0; i < p_size; i++){
    max += getFitness(population[i]);
  }
  select = getRandomDouble(rng)*max;
  i = -1
  while (select >= 0):
    i += 1
    select -= getFitness(population[i]);

  return clone_individual(population[i], true);
}

void getElite(const Individual population[], int p_size, Individual elite[], int e_size){
  int i;
  for (i = 0; i < e_size; i++) {
    elite[i] = clone_individual(population[i], true);
  }
}



void getOffspring(RNG rng, const Individual population[], int p_size, Individual offspring[] ) {
  int isCross = getRandomDouble(rng) < 0.7;
  int isMut1 = getRandomDouble(rng) < 0.5;
  int isMut2 = getRandomDouble(rng) < 0.5;
  Individual parent1 = roulette(rng, population, p_size);
  Individual parent2 = roulette(rng, population, p_size);

  if (isCross){
    STXO(rng, parent1, parent2);
  }
  if (isMut1){
    STMUT(rng, parent1);
  }
  if (isMut2){
    STMUT(rng, parent1);
  }

  offspring[0] = parent1;
  offspring[1] = parent2;
}

void STXO(RNG rng, Individual parent1, Individual parent2 ) {

  double[] a1 = parent1.weights;
  double[] a2 = parent2.weights;
  int cut = getRandomInt(rng, parent1.n_weights);
  int i;
  double ax;

  for(i = cut; i < parent1.n_weights; i++){
    ax = parent1.weights[i];
    parent1.weights[i] = parent2.weights[i];
    parent2.weights[i] = ax;
  }
  
}

void STMUT(RNG rng, Individual parent){

  double[] a = parent.weights;
  int i;
  double isFlip;

  for(i = 0; i < parent.n_weights; i++){
    isFlip = getRandomDouble(rng) < 1/parent.n_weights;
    if (isFlip){
      parent.weights[i] = getRandomDouble(rng);
    }
  }
}

