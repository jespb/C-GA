/*
 * By using this file, you are agreeing to this product's EULA
 *
 * This product can be obtained in https://github.com/jespb/C-GA
 *
 * Copyright ©2023 J. E. Batista
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <individual.h>




Individual individual_create(RNG *rng, int n_weights){
  Individual ind;
  int i;

  ind.n_weights = n_weights;
  for(i = 0; i < n_weights; i++){
    ind.weights[i] = getRandomDouble(rng);
  }
  ind.fitness = -1;
  ind.string[0] = '\0';

  return ind;

}

Individual individual_clone(Individual *ind){
  Individual copy;
  int i;

  copy.n_weights = ind->n_weights;
  for(i = 0; i < ind->n_weights; i++){
    copy.weights[i] = ind->weights[i];
  }

  return copy;
}


/*
 * The individuals MUST have calculates the fitness beforehand
 * bigger fitness = better
 */
int compare(const void *a, const void *b) {
  Individual *this  = (Individual*)((const long int *)a)[0];
  Individual *other = (Individual*)((const long int *)b)[0];
  return this->fitness > other->fitness;
}


double getFitness(Individual *ind, double **training_X, double *training_Y, int n_samples){
  if (ind->fitness < 0){
    ind->training_X = training_X;
    ind->training_Y = training_Y;
    ind->n_samples = n_samples;

    double rmse = getRMSE(ind, training_X, training_Y, n_samples);

    ind->fitness = 1/ rmse; // Inverted 

  }
  return ind->fitness;
}




double getAccuracy(Individual *ind, double **X, double *Y, int n_samples){
  int predictions[ind->n_samples];
  int hits, i;

  individual_predict_classification(ind, X, n_samples, predictions);

  hits = 0;
  for(i = 0; i < n_samples; i++){
    if( predictions[i] == Y[i] ){
      hits ++;
    }
  }

  return hits/(double)n_samples;
}


double getRMSE(Individual *ind, double **X, double *Y, int n_samples){
  double rmse = 0;
  double pred;
  int i;
  for( i = 0; i < n_samples; i++){
    pred = individual_predict(ind, X[i]);
    rmse += pow(pred - Y[i],2);
  }

  return pow(rmse/n_samples, 0.5);
}


double individual_predict(Individual *ind, double *sample){
  double ax = 0;
  for(int i = 0; i < ind->n_weights; i++){
    ax += ind->weights[i]*sample[i];
  }
  return ax;
}

void individual_predict_regression(Individual *ind, double **X, int n_samples, double predictions[]){
  for(int i = 0; i < n_samples; i++){
    predictions[i] = individual_predict(ind, X[i]);
  }
}

void individual_predict_classification(Individual *ind, double **X, int n_samples, int predictions[]){
  for(int i = 0; i < n_samples; i++){
    predictions[i] = individual_predict(ind, X[i]) > 0.5 ? 1 : 0;
  }
}

char *individual_toString(Individual *ind){
  char str[100];
  if( strlen(ind->string) == 0){
    strcat(ind->string, "[");
    for(int i = 0; i < ind->n_weights; i++){
      sprintf(str, "%f, ", ind->weights[i]);
      strcat(ind->string, str);

    }
    int length = strlen(ind->string);
    ind->string[length-2] = ']';
    ind->string[length-2] = '\0';
  }
  return ind->string;
}

void individual_destroy(Individual *ind){
  //TODO
}



