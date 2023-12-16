/*
 * By using this file, you are agreeing to this product's EULA
 *
 * This product can be obtained in https://github.com/jespb/C-GA
 *
 * Copyright ©2021-2022 J. E. Batista
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GA.h>
#include <geneticoperators.h>
#include <individual.h>
#include <RNG.h>

GA ga_create(char **terminals, int n_terminals, int population_size, int max_generation,
                             int elitism_size, int threads, int verbose,
                             double **training_X, double *training_Y, int n_training_samples, 
                             double **test_X, double *test_Y, int n_test_samples, int seed) {

  GA ga;// = malloc(sizeof(GA));

  initRandomGenerator(seed);

  ga.population = malloc(sizeof(Individual) * population_size);
  ga.bestIndividual = malloc(sizeof(Individual));

  ga.n_training_samples = n_training_samples;
  ga.training_X = training_X;
  ga.training_Y = training_Y;

  ga.n_test_samples = n_test_samples;
  ga.test_X = test_X;
  ga.test_Y = test_Y;

  ga.n_terminals = n_terminals;
  ga.terminals = terminals;

  ga.population_size = population_size;
  ga.max_generation = max_generation;
  ga.elitism_size = elitism_size;
  ga.threads = threads;
  ga.verbose = verbose;

  //ga.trainingAccuracyOverTime = calloc(max_generation + 1, sizeof(double));
  //ga.testAccuracyOverTime = calloc(max_generation + 1, sizeof(double));
  //ga.trainingRMSEOverTime = calloc(max_generation + 1, sizeof(double));
  //ga.testRMSEOverTime = calloc(max_generation + 1, sizeof(double));
  //ga.fitnessOverTime = calloc(max_generation + 1, sizeof(double));
  //ga.timeOverTime = calloc(max_generation + 1, sizeof(double));

  ga.currentGeneration = 0;

  //ga.population =
  //    malloc(sizeof(struct individual_t *) * (population_size + 1));
  
  for (int i = 0; i < population_size; ++i) {
    ga.population[i] = individual_create(n_terminals);
  }

  *ga.bestIndividual = individual_clone(&ga.population[0]);

  return ga;
}


int stoppingCriteria(GA *ga) {
  int genLimit = ga->currentGeneration >= ga->max_generation;
  int perfectTraining =
      getFitness(ga->bestIndividual, ga->training_X, ga->training_Y, ga->n_training_samples) == 1;

  return genLimit || perfectTraining;
}

void fit(GA *ga) {
  if (ga->verbose) {
    printf("Training a model with the following parameters:\n");
    printf("  > Population Size: %d\n", ga->population_size);
    printf("  > Max Generation: %d\n", ga->max_generation);
    printf("  > Elitism Size: %d\n", ga->elitism_size);
    printf("  > Threads (not implemented): %d\n", ga->threads);
    printf("\n");
    printf("Running Log:\n");
  }

  while (ga->currentGeneration < ga->max_generation) {
    long duration = 0;

    if (!stoppingCriteria(ga)) { 
      if (ga->verbose && ga->currentGeneration % 1 == 0)
        printf("Generation %3d:", ga->currentGeneration);

      clock_t before = clock();
      nextGeneration(ga);
      clock_t after = clock();
      duration = (after - before) * 1000.0 / CLOCKS_PER_SEC;

      if (ga->verbose && ga->currentGeneration % 1 == 0)
        printf("  > Execution Time: %ldms\n", duration);
    }

    ga->trainingAccuracyOverTime[ga->currentGeneration] =
        getAccuracy(ga->bestIndividual,
                    ga->training_X, ga->training_Y, ga->n_training_samples);
    ga->testAccuracyOverTime[ga->currentGeneration] =
        getAccuracy(ga->bestIndividual, ga->test_X,
                    ga->test_Y, ga->n_test_samples);
    ga->trainingRMSEOverTime[ga->currentGeneration] =
        getRMSE(ga->bestIndividual,
                ga->training_X, ga->training_Y, ga->n_training_samples);
    ga->testRMSEOverTime[ga->currentGeneration] =
        getRMSE(ga->bestIndividual, ga->test_X,
                ga->test_Y, ga->n_test_samples);
    ga->fitnessOverTime[ga->currentGeneration] =
        getFitness(ga->bestIndividual,
                   ga->training_X, ga->training_Y, ga->n_training_samples);
    ga->timeOverTime[ga->currentGeneration] = duration / 1000.0;

    // printf("%f\n", getAccuracy(ga->bestIndividual,
    // ga->n_training_samples, ga->training_X, ga->training_Y));

    ga->currentGeneration++;
  }
}

/*
qsort( a, 6, sizeof(int), compare )

// USE this to sort the population
*/
void nextGeneration(GA *ga) {
  // Calculate the individual's fitness
  for (int i = 0; i < ga->population_size; ++i) {
    getFitness(&ga->population[i], 
               ga->training_X, ga->training_Y, ga->n_training_samples);
    // printf("%ld\n", ga->population[i]);
  }

  // Sorts the population
  qsort(ga->population, ga->population_size,
        sizeof(struct individual_t *), compare);

  // Updates the best individual
  if (ga->population[0].fitness > ga->bestIndividual->fitness) {
    *ga->bestIndividual = individual_clone(&ga->population[0]);
  }

  // Generates next generation
  Individual new_population[1000];
  Individual elite[100];
  Individual offspring[10];
  getElite(ga->population, ga->population_size, elite, ga->elitism_size);
  for (int i = 0; i < ga->elitism_size; ++i) {
    new_population[i] = elite[i];
  }

  for (int i = ga->elitism_size; i < ga->population_size; i+=2) {
    getOffspring(ga->population, ga->population_size, offspring);
    new_population[i]   = offspring[0];
    if (i+1 < ga->population_size){
      new_population[i+1] = offspring[1];
    }
  }

  // Deletes and replaces the previous generation
  for (int i = 0; i < ga->population_size; ++i) {
    ga->population[i] = new_population[i];
  }

  // Generation Log
  if (ga->verbose && ga->currentGeneration % 1 == 0)
    printf("  > Training: %.4f  // Test: %.4f      ",
           getAccuracy(ga->bestIndividual, ga->training_X, ga->training_Y, ga->n_training_samples),
           getAccuracy(ga->bestIndividual, ga->test_X, ga->test_Y, ga->n_test_samples));
}



void predict_regression(GA *ga, int n_samples, double **x, double *prediction) {
  individual_predict_regression(ga->bestIndividual, x, n_samples, prediction);
}

void predict_classification(GA *ga, int n_samples, double **x, int *prediction) {
  individual_predict_classification(ga->bestIndividual, x, n_samples, prediction);
}

Individual *getBestIndividual(GA *ga) {
  return ga->bestIndividual;
}

double *getTrainingAccuracyOverTime(GA *ga) {
  return ga->trainingAccuracyOverTime;
}

double *getTestAccuracyOverTime(GA *ga) {
  return ga->testAccuracyOverTime;
}

double *getTrainingRMSEOverTime(GA *ga) {
  return ga->trainingRMSEOverTime;
}

double *getTestRMSEOverTime(GA *ga) {
  return ga->testRMSEOverTime;
}

double *getFitnessOverTime(GA *ga) {
  return ga->fitnessOverTime;
}

double *getTimeOverTime(GA *ga) { 
  return ga->timeOverTime; 
}

char *toString_GA(GA *ga) {
  return individual_toString(ga->bestIndividual);
}
