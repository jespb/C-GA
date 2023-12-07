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

  ga.trainingAccuracyOverTime = calloc(max_generation + 1, sizeof(double));
  ga.testAccuracyOverTime = calloc(max_generation + 1, sizeof(double));
  ga.trainingRMSEOverTime = calloc(max_generation + 1, sizeof(double));
  ga.testRMSEOverTime = calloc(max_generation + 1, sizeof(double));
  ga.fitnessOverTime = calloc(max_generation + 1, sizeof(double));
  ga.timeOverTime = calloc(max_generation + 1, sizeof(double));

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

void ga_destroy(struct GA_t *ga) {
  free(ga->trainingAccuracyOverTime);
  free(ga->testAccuracyOverTime);
  free(ga->trainingWaFOverTime);
  free(ga->testWaFOverTime);
  free(ga->trainingKappaOverTime);
  free(ga->testKappaOverTime);
  free(ga->trainingRMSEOverTime);
  free(ga->testRMSEOverTime);
  free(ga->fitnessOverTime);
  free(ga->sizeOverTime);
  free(ga->timeOverTime);

  for (int i = 0; i < ga->population_size; i++) {
    individual_destroy(ga->population[i]);
  }
  free(ga->population);

  individual_destroy(ga->bestIndividual);

  free(ga);
}

_Bool stoppingCriteria(struct GA_t *ga) {
  _Bool genLimit = ga->currentGeneration >= ga->max_generation;
  _Bool perfectTraining =
      getFitness(ga->bestIndividual, ga->n_training_samples,
                 ga->training_X, ga->training_Y) == 1;

  return genLimit || perfectTraining;
}

void fit(struct GA_t *ga) {
  if (ga->verbose) {
    printf("Training a model with the following parameters:\n");
    printf("  > Population Size: %d\n", ga->population_size);
    printf("  > Max Generation: %d\n", ga->max_generation);
    printf("  > Tournament Size: %d\n", ga->tournament_size);
    printf("  > Elitism Size: %d\n", ga->elitism_size);
    printf("  > Depth Limit: %d\n", ga->limit_depth);
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
void nextGeneration(struct GA_t *ga) {
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
  struct individual_t **new_population =
      calloc(ga->population_size + 1, sizeof(struct individual_t *));
  struct individual_t **elite =
      getElite(ga->population_size, ga->population, ga->elitism_size);
  for (int i = 0; i < ga->elitism_size; ++i) {
    new_population[i] = elite[i];
  }
  free(elite);

  for (int i = ga->elitism_size; i < ga->population_size;) {
    struct individual_t **offspring = getOffspring(
        ga->population_size, ga->population, ga->tournament_size);
    struct individual_t **afterDiscard =
        discardDeep(offspring, ga->limit_depth);
    free(offspring);

    int j = 0;
    while (afterDiscard[j] != NULL) {
      if (i + j < ga->population_size) {
        new_population[i + j] = afterDiscard[j];
      } else {
        individual_destroy(afterDiscard[j]);
      }
      j++;
    }

    free(afterDiscard);

    i += j;
  }

  // Deletes and replaces the previous generation
  for (int i = 0; i < ga->population_size; ++i) {
    individual_destroy(ga->population[i]);
  }
  free(ga->population);
  ga->population = new_population;

  // Generation Log
  if (ga->verbose && ga->currentGeneration % 1 == 0)
    printf("  > Training: %.4f  // Test: %.4f      ",
           getAccuracy(ga->bestIndividual, ga->n_training_samples,
                       ga->training_X, ga->training_Y),
           getAccuracy(ga->bestIndividual, ga->n_test_samples,
                       ga->test_X, ga->test_Y));
}

/*


                # Update best individual
                if self.population[0] > self.bestIndividual:
                        self.bestIndividual = self.population[0]

                # Generating Next Generation
                newPopulation = []
                newPopulation.extend(getElite(self.population,
   self.elitism_size)) while len(newPopulation) < self.population_size:
                        offspring = getOffspring(self.population,
   self.tournament_size) offspring = discardDeep(offspring, self.limit_depth)
                        newPopulation.extend(offspring)
                self.population = newPopulation[:self.population_size]



                # Debug
                if self.verbose and self.currentGeneration%5==0:
                        if not self.Te_x is None:
                                print("   > Gen
   #"+str(self.currentGeneration)+":  Tr-Acc: "+ "%.6f"
   %self.bestIndividual.getAccuracy(self.Tr_x, self.Tr_y)+" // Te-Acc: "+ "%.6f"
   %self.bestIndividual.getAccuracy(self.Te_x, self.Te_y) + " // Time: " +
   str(end- begin) ) else: print("   > Gen #"+str(self.currentGeneration)+":
   Tr-Acc: "+ "%.6f" %self.bestIndividual.getAccuracy(self.Tr_x, self.Tr_y))


*/

double *predict_regression(struct GA_t *ga, int n_samples, double **x) {
  return calculate_individual(ga->bestIndividual, n_samples, x);
}

int *predict_classification(struct GA_t *ga, int n_samples, double **x) {
  return predict_individual(ga->bestIndividual, n_samples, x);
}

struct individual_t *getBestIndividual(struct GA_t *ga) {
  return ga->bestIndividual;
}

double *getTrainingAccuracyOverTime(struct GA_t *ga) {
  return ga->trainingAccuracyOverTime;
}

double *getTestAccuracyOverTime(struct GA_t *ga) {
  return ga->testAccuracyOverTime;
}

double *getTrainingRMSEOverTime(struct GA_t *ga) {
  return ga->trainingRMSEOverTime;
}

double *getTestRMSEOverTime(struct GA_t *ga) {
  return ga->testRMSEOverTime;
}

double *getFitnessOverTime(struct GA_t *ga) {
  return ga->fitnessOverTime;
}

int *getSizeOverTime(struct GA_t *ga) { return ga->sizeOverTime; }

double *getTimeOverTime(struct GA_t *ga) { return ga->timeOverTime; }

char *toString_GA(struct GA_t *ga) {
  return toString_individual(ga->bestIndividual);
}