#include <stdio.h>
#include <RNG.h>
#include <individual.h>
#include <stdlib.h>
#include <string.h>

#define LEN 5

void printArray(int array[], int length){
	for(int i = 0; i < length-1; i++){
		printf("%d, ", array[i]);
	}
	printf("%d\n", array[length-1]);
}

int min(int array[], int length){
	int v = array[0];
	for(int i = 0; i < length; i++){
		if (array[i] < v){
			v = array[i];
		}
	}
	return v;
}

int max(int array[], int length){
	int v = array[0];
	for(int i = 0; i < length; i++){
		if (array[i] > v){
			v = array[i];
		}
	}
	return v;
}



int testRNG(){
	int valid = 1;
	int attempts = 100000;
	int samples[LEN] = {};
	int changes[LEN] = {};

    initRandomGenerator(0);

    int previous = 0;
	for (int i = 0; i < attempts; i++){
		int x = getRandomInt(LEN);
		samples[ x ] ++;
		changes[abs(previous-x)]++;
		previous = x;
	}

	double diff = (double)max(samples, LEN)/min(samples, LEN);
	if (diff < 0.95 || diff > 1.05){
		valid = 0;
	}

	valid &= changes[1]>changes[0];
	for (int i = 1; i < LEN-1; i++){
		valid &= changes[i]>changes[i+1];
	}

	return valid;

}

int compareIntArrays(int v1[], int v2[], int length){
	int valid = 1;
	for(int i = 0; i < length && valid; i++){
		valid &= v1[i] == v2[i];
	}
	return valid;
}

int compareDoubleArrays(double v1[], double v2[], int length){
	int valid = 1;
	for(int i = 0; i < length && valid; i++){
		valid &= abs(v1[i] - v2[i]) < 0.001;
	}
	return valid;
}


int testIndividual(){
	int valid = 1;

	double sample[3] = {3,4,5};

	double **dataset = malloc(3* sizeof(double*)); 
	dataset[0] = malloc(3*sizeof(double));
	dataset[0][0] = -3; 
	dataset[0][1] = -2; 
	dataset[0][2] = -1; 
	dataset[1] = malloc(3*sizeof(double));
	dataset[1][0] = 0.49; 
	dataset[1][1] = 0; 
	dataset[1][2] = 0; 
	dataset[2] = malloc(3*sizeof(double));
	dataset[2][0] = 0.51; 
	dataset[2][1] = 0; 
	dataset[2][2] = 0; 
	dataset[3] = malloc(3*sizeof(double));
	dataset[3][0] = 3; 
	dataset[3][1] = 4; 
	dataset[3][2] = 5; 

	double results_reg[4] = {-10, 0.49, 0.51, 26};
	int results_class[4] = {0,0,1,1}; 
	double results_class_double[4] = {0,0,1,1}; 

	Individual ind = individual_create(3);
	ind.weights[0] = 1;
	ind.weights[1] = 2;
	ind.weights[2] = 3;
	Individual ind2 = individual_clone(&ind);

	if (strcmp(individual_toString(&ind), "[1.000000, 2.000000, 3.000000]")!=0){
		printf("(Error at: individual_toString)... ");
		valid = 0;
	}

	if (strcmp(individual_toString(&ind), individual_toString(&ind2))!=0){
		printf("%s\n",individual_toString(&ind) );
		printf("%s\n",individual_toString(&ind2) );
		printf("(Error at: individual_clone)... ");
		valid = 0;
	}

	if ( abs(26 - individual_predict(&ind, sample)) > 0.001 ){
		printf("(Error at individual_predict: EXPECTED:26 GOT:%f ", individual_predict(&ind, sample));
		valid = 0;
	}

	double predictions_reg[4];
	individual_predict_regression(&ind, dataset, 4, predictions_reg);
	if ( compareDoubleArrays(predictions_reg, results_reg, 4) == 0){
		printf("(Error at: individual_predict_regression)... ");
		valid = 0;
	}

	int predictions_class[4];
	individual_predict_classification(&ind, dataset, 4, predictions_class);
	if ( compareIntArrays(predictions_class, results_class, 4) == 0){
		printf("(Error at: individual_predict_classification)... ");
		valid = 0;
	}


	if ( abs(1 - getAccuracy(&ind, dataset, results_class_double, 4) ) >  0.001 ){
		printf("(Error at: getAccuracy)... GOT: %f ", getAccuracy(&ind, dataset, results_class_double, 4));
		valid = 0;
	}

	if ( abs(0 - getRMSE(&ind, dataset, results_reg, 4) ) >  0.001 ){
		printf("(Error at: getRMSE)... GOT: %f ", getRMSE(&ind, dataset, results_reg, 4));
		valid = 0;
	}

	if ( abs(getFitness(&ind, dataset, results_reg, 4) - 1/getRMSE(&ind, dataset, results_reg, 4) ) >  0.001 ){
		printf("(Error at: getFitness)... GOT: %f ", getFitness(&ind, dataset, results_reg, 4));
		valid = 0;
	}


	return valid;

}




int main(){
	int valid;

	printf("Testes ao RNG.c ...        ");
	valid = testRNG();
	printf("%s\n" , valid ? "[DONE]" : "[ERROR]");


	printf("Testes ao Individual.c ...        ");
	valid = testIndividual();
	printf("%s\n" , valid ? "[DONE]" : "[ERROR]");





	return 0;
}