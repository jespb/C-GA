/*
 * By using this file, you are agreeing to this product's EULA
 *
 * This product can be obtained in https://github.com/jespb/C-GA
 *
 * Copyright ©2023 J. E. Batista
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Number of models in the population
int POPULATION_SIZE = 500;

// Maximum number of iterations
int MAX_GENERATION = 100;

// Fraction of the dataset to be used as training (used by main.c)
double TRAIN_FRACTION = 0.70;

// Number of best individuals to be automatically moved to the next generation
int ELITISM_SIZE = 1;

// Shuffle the dataset (used by main.c)
_Bool SHUFFLE = 1;

// Number of runs (used by main.c)
int RUNS = 30;

// Verbose
_Bool VERBOSE = 1;

// Number of CPU Threads to be used (not implemented)
int THREADS = 1;

char *DATASETS_DIR = "datasets/";
char *OUTPUT_DIR = "results/";

int N_DATASETS = 1;

char **DATASETS;

char *OUTPUT = "Classification";

int string_array_destroy(char **sa) {
  for (int i = 0; sa[i] != NULL; i++) {
    free(sa[i]);
  }
  free(sa);
  return 0;
}

int count(char *s, char *token) {
  int count = 0;
  char *tempstr = calloc(strlen(s) + 1, sizeof(char));
  strcpy(tempstr, s);
  char *tmp = strtok(tempstr, token);
  while (tmp != NULL) {
    tmp = strtok(NULL, token);
    count++;
  }
  free(tempstr);
  return count;
}

char **split(char *s, int size, char *token) {
  if (size == -1) {
    size = count(s, token);
  }

  char *tempstr = calloc(strlen(s) + 1, sizeof(char));
  strcpy(tempstr, s);
  char *tmp = strtok(tempstr, token);

  char **ret;
  ret = calloc(size + 1, sizeof(char *));

  int i = 0;
  while (i < size) {
    // printf("%ld\n",strlen(tmp));
    ret[i] = calloc(strlen(tmp) + 1, sizeof(char));
    strcpy(ret[i], tmp);
    // ret[i] = tmp;
    i++;
    tmp = strtok(NULL, token);
  }
  while (tmp != NULL) {
    tmp = strtok(NULL, token);
  }
  free(tempstr);
  return ret;
}

void initArguments() {
  printf("initArguments: DATASETS\n");
  N_DATASETS = 1;
  DATASETS = split("heart.csv", -1, ";");
}

void freeArguments() {
  string_array_destroy(DATASETS);
}

void updateArguments(int argc, char *argv[]) {
  printf("Updating Arguments...\n");
  for (int i = 0; i < argc; i++) {

    if (!strcmp(argv[i], "-dsdir")) {
      DATASETS_DIR = argv[i + 1];
      printf("  Dataset Directory: %s\n", DATASETS_DIR);
    }

    if (!strcmp(argv[i], "-odir")) {
      OUTPUT_DIR = argv[i + 1];
      printf("  Output Directory: %s\n", OUTPUT_DIR);
    }

    if (!strcmp(argv[i], "-d")) {
      N_DATASETS = count(argv[i + 1], ";");
      free(DATASETS);
      DATASETS = calloc(N_DATASETS, sizeof(char *));
      char **spl = split(argv[i + 1], N_DATASETS, ";");
      for (int j = 0; j < N_DATASETS; j++) {
        DATASETS[j] = spl[j];
        printf("  Datasets: %s\n", DATASETS[j]);
      }
    }

    if (!strcmp(argv[i], "-runs")) {
      RUNS = atoi(argv[i + 1]);
      printf("  Number of Runs: %d\n", RUNS);
    }

    if (!strcmp(argv[i], "-ps")) {
      POPULATION_SIZE = atoi(argv[i + 1]);
      printf("  Population Size: %d\n", POPULATION_SIZE);
    }

    if (!strcmp(argv[i], "-mg")) {
      MAX_GENERATION = atoi(argv[i + 1]);
      printf("  Maximum Number of Generations: %d\n", MAX_GENERATION);
    }

    if (!strcmp(argv[i], "-tf")) {
      TRAIN_FRACTION = atof(argv[i + 1]);
      printf("  Train Fraction: %.2f\n", TRAIN_FRACTION);
    }

    if (!strcmp(argv[i], "-es")) {
      ELITISM_SIZE = atoi(argv[i + 1]);
      printf("  Elitism Size: %d\n", ELITISM_SIZE);
    }

    if (!strcmp(argv[i], "-dontshuffle")) {
      SHUFFLE = 0;
      printf("  Shuffle: %d\n", SHUFFLE);
    }

    if (!strcmp(argv[i], "-s")) {
      VERBOSE = 0;
      printf("  Verbose: %d\n", VERBOSE);
    }

    if (!strcmp(argv[i], "-t")) {
      THREADS = atoi(argv[i + 1]);
      printf("  Number of Threads: %d\n", THREADS);
    }
  }
  printf("[DONE]\n\n");
}