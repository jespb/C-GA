/* 
* By using this file, you are agreeing to this product's EULA
*
* This product can be obtained in https://github.com/jespb/C-GA
*
* Copyright ©2023 J. E. Batista
*/

#include <individual.h>

#ifndef _GA_H
#define _GA_H

#define MAX_GEN 1000

typedef struct {
	int n_training_samples;
	double **training_X;
	double *training_Y;

	int n_test_samples;
	double **test_X;
	double *test_Y;

	int n_terminals;
	char** terminals;

	int population_size;
	int max_generation;
	int elitism_size;
	int threads;

	int verbose;

	Individual population[1000];
	Individual bestIndividual;

	int currentGeneration;

	double trainingAccuracyOverTime[MAX_GEN];
	double testAccuracyOverTime[MAX_GEN]; 
	double trainingRMSEOverTime[MAX_GEN]; 
	double testRMSEOverTime[MAX_GEN];
	double fitnessOverTime[MAX_GEN];
	double timeOverTime[MAX_GEN]; 

	RNG rng;
} GA;

GA ga_create(char** terminals, int n_terminals, int population_size, int max_generation, 
	int elitism_size, int threads, int verbose,	double **training_X, double *training_Y, 
	int n_training_samples, double **test_X, double *test_Y, int n_test_samples);

void ga_destroy(GA *GenAlg);

char* toString_ga(GA *GenAlg);

int stoppingCriteria(GA *GenAlg);

void fit(GA *GenAlg);

void nextGeneration(GA *GenAlg);


void predict_regression(GA *GenAlg, int n_samples, double **x, double *predictions);
void predict_classification(GA *GenAlg, int n_samples, double **x, int *predictions);

Individual* getBestIndividual(GA *GenAlg);

double* getTrainingAccuracyOverTime(GA *GenAlg);
double* getTestAccuracyOverTime(GA *GenAlg);
double* getTrainingRMSEOverTime(GA *GenAlg);
double* getTestRMSEOverTime(GA *GenAlg);
double* getFitnessOverTime(GA *GenAlg);
double* getTimeOverTime(GA *GenAlg);

#endif
