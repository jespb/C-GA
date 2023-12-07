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

GA ga_create(char **terminals, int n_terminals, int population_size, int max_generation,
                             int elitism_size, int threads, int verbose,
                             double **training_X, double *training_Y, int n_training_samples, 
                             double **test_X, double *test_Y, int n_test_samples) {

  GA ga;// = malloc(sizeof(GA));

  initRandomGenerator(ga.rng, 42); // seed 42

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
    ga.population[i] = individual_create(n_operators, operators,
                                             n_terminals, terminals, max_depth);
  }

  ga->bestIndividual = clone_individual(ga->population[0], 1);

  return ga;
}

void ga_destroy(GA *ga) {
  //free(ga->trainingAccuracyOverTime);
  //free(ga->testAccuracyOverTime);
  //free(ga->trainingRMSEOverTime);
  //free(ga->testRMSEOverTime);
  //free(ga->fitnessOverTime);
  //free(ga->timeOverTime);

  for (int i = 0; i < ga->population_size; i++) {
    individual_destroy(ga->population[i]);
  }
  //free(ga->population);

  individual_destroy(ga->bestIndividual);

  free(ga);
}

int stoppingCriteria(GA *ga) {
  int genLimit = ga->currentGeneration >= ga->max_generation;
  int perfectTraining =
      getFitness(ga->bestIndividual, ga->n_training_samples,
                 ga->training_X, ga->training_Y) == 1;

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
        getAccuracy(ga->bestIndividual, ga->n_training_samples,
                    ga->training_X, ga->training_Y);
    ga->testAccuracyOverTime[ga->currentGeneration] =
        getAccuracy(ga->bestIndividual, ga->n_test_samples, ga->test_X,
                    ga->test_Y);
    ga->trainingWaFOverTime[ga->currentGeneration] =
        getWaF(ga->bestIndividual, ga->n_training_samples,
               ga->training_X, ga->training_Y);
    ga->testWaFOverTime[ga->currentGeneration] =
        getWaF(ga->bestIndividual, ga->n_test_samples, ga->test_X,
               ga->test_Y);
    ga->trainingKappaOverTime[ga->currentGeneration] =
        getKappa(ga->bestIndividual, ga->n_training_samples,
                 ga->training_X, ga->training_Y);
    ga->testKappaOverTime[ga->currentGeneration] =
        getKappa(ga->bestIndividual, ga->n_test_samples, ga->test_X,
                 ga->test_Y);
    ga->trainingRMSEOverTime[ga->currentGeneration] =
        getRMSE(ga->bestIndividual, ga->n_training_samples,
                ga->training_X, ga->training_Y);
    ga->testRMSEOverTime[ga->currentGeneration] =
        getRMSE(ga->bestIndividual, ga->n_test_samples, ga->test_X,
                ga->test_Y);
    ga->fitnessOverTime[ga->currentGeneration] =
        getFitness(ga->bestIndividual, ga->n_training_samples,
                   ga->training_X, ga->training_Y);
    ga->sizeOverTime[ga->currentGeneration] =
        getSize(ga->bestIndividual);
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
    getFitness(ga->population[i], ga->n_training_samples,
               ga->training_X, ga->training_Y);
    // printf("%ld\n", ga->population[i]);
  }

  // Sorts the population
  qsort(ga->population, ga->population_size,
        sizeof(struct individual_t *), compare);

  // Updates the best individual
  if (ga->population[0]->fitness > ga->bestIndividual->fitness) {
    individual_destroy(ga->bestIndividual);
    ga->bestIndividual = clone_individual(ga->population[0], 1);
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
    getOffspring(ga->rng, ga->population, ga->population_size, offspring);
    new_population[i]   = offspring[0];
    if (i+1 < ga->population_size){
      new_population[i+1] = offspring[1];
    }else{
      individual_destroy(offspring[1]);
    }
  }

  // Deletes and replaces the previous generation
  for (int i = 0; i < ga->population_size; ++i) {
    individual_destroy(ga->population[i]);
    ga->population[i] = new_population[i];
  }

  // Generation Log
  if (ga->verbose && ga->currentGeneration % 1 == 0)
    printf("  > Training: %.4f  // Test: %.4f      ",
           getAccuracy(ga->bestIndividual, ga->training_X, ga->training_Y, ga->n_training_samples),
           getAccuracy(ga->bestIndividual, ga->test_X, ga->test_Y, ga->n_test_samples));
}



double *predict_regression(GA *ga, int n_samples, double **x) {
  return calculate_individual(ga->bestIndividual, n_samples, x);
}

int *predict_classification(GA *ga, int n_samples, double **x) {
  return predict_individual(ga->bestIndividual, n_samples, x);
}

struct individual_t *getBestIndividual(GA *ga) {
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

int *getSizeOverTime(GA *ga) { return ga->sizeOverTime; }

double *getTimeOverTime(GA *ga) { return ga->timeOverTime; }

char *toString_GA(GA *ga) {
  return toString_individual(ga->bestIndividual);
}
