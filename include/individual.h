/* 
* By using this file, you are agreeing to this product's EULA
*
* This product can be obtained in https://github.com/jespb/C-GA
*
* Copyright ©2023 J. E. Batista
*/

#ifndef _INDIVIDUAL_H
#define _INDIVIDUAL_H

typedef struct{
  double **training_X;
  double *training_Y;
  int n_samples;

  double *weights;
  int n_weights;

  double fitness;
  char *string;
} Individual;


Individual *individual_create(int n_weights);

Individual *individual_clone(Individual *ind, int copyFitness);


int compare(const void * a, const void * b);
/*
int compare( const void* a, const void* b){
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

qsort( a, 6, sizeof(int), compare )
*/


double getFitness(Individual *ind, double **training_X, double *training_Y, int n_samples);

double getAccuracy(Individual *ind, double **X, double *Y, int n_samples);
double getRMSE(Individual *ind,     double **X, double *Y, int n_samples);

double individual_predict(Individual *ind, double *sample);
void individual_predict_regression(Individual *ind, double **X, int n_samples, double predictions[]);
void individual_predict_classification(Individual *ind, double **X, int n_samples, int predictions[]);

char *individual_toString(Individual *ind);
void individual_destroy(Individual *ind);

#endif


