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
#include <RNG.h>




Individual roulette(Individual population[], int p_size){
  double max = 0, select;
  int i;
  for(i = 0; i < p_size; i++){
    max += getFitness(&population[i], NULL, NULL, -1);
  }
  select = getRandomDouble()*max;
  i = -1;
  while(select >= 0){
    i += 1;
    select -= getFitness(&population[i], NULL, NULL, -1);
  }

  return individual_clone(&population[i]);
}

void getElite(Individual population[], int p_size, Individual elite[], int e_size){
  int i;
  for (i = 0; i < e_size; i++) {
    elite[i] = individual_clone(&population[i]);
  }
}



void getOffspring(Individual population[], int p_size, Individual offspring[] ) {
  int isCross = getRandomDouble() < 0.7;
  int isMut1 = getRandomDouble() < 0.5;
  int isMut2 = getRandomDouble() < 0.5;
  Individual parent1 = roulette(population, p_size);
  Individual parent2 = roulette(population, p_size);

  if (isCross){
    STXO(&parent1, &parent2);
  }
  if (isMut1){
    STMUT(&parent1);
  }
  if (isMut2){
    STMUT(&parent1);
  }

  offspring[0] = parent1;
  offspring[1] = parent2;
}

void STXO(Individual *parent1, Individual *parent2 ) {

  int cut = getRandomInt(parent1->n_weights);
  int i;
  double ax;

  for(i = cut; i < parent1->n_weights; i++){
    ax = parent1->weights[i];
    parent1->weights[i] = parent2->weights[i];
    parent2->weights[i] = ax;
  }
  
}

void STMUT(Individual *parent){

  int i;
  double isFlip;

  for(i = 0; i < parent->n_weights; i++){
    isFlip = getRandomDouble() < 1/parent->n_weights;
    if (isFlip){
      parent->weights[i] = getRandomDouble();
    }
  }
}


