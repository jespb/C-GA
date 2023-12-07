# C-GA
This is a, easy-to-use, scikit-learn inspired version of the Genetic Algorithm (GA).


By using this file, you are agreeing to this product's EULA
This product can be obtained in https://github.com/jespb/C-GA
Copyright ©2023 J. E. Batista


This file contains information about the command and flags used in the stand-alone version of this implementation and an explanation on how to import, use and edit this implementation.


This implementation of GA can be used in a stand-alone fashion using the following command and flags:

$ exec_GA
	
	[-d datasets] 
		- This flag expects a set of csv dataset names separated by ";" (e.g., a.csv;b.csv)
		- By default, the heart.csv dataset is used		

	[-dsdir dir] 
		- States the dataset directory. 
		- By default "datasets/" is used 
		- Use "-dsdir ./" for the root directory	

	[-es elite_size]
		- This flag expects an integer with the elite size;
		- By default, the elite has size 1.	

	[-mg max_generation]
		- This flag expects an integer with the maximum number of generations;
		- By default, this value is set to 100.

	[-odir dir] 
		- States the output directory. 
		- By default "results/" is used 
		- Use "-odir ./" for the root directory

	[-ps population_size]
		- This flag expects an integer with the size of the population;
		- By default, this value is set to 500.

	[-runs number_of_runs] 
		- This flag expects an integer with the number of runs to be made;
		- By default, this values is set to 30
	
	[-tf train_fraction]
		- This flag expects a float [0;1] with the fraction of the dataset to be used in training;
		- By default, this value is set to 0.70

	[-t number_of_threads]
		- This flag expects an integer with the number of threads to use while evaluating the population;
		- If the value is set to 1, the multiprocessing library will not be used 
		- By default, this value is set to 1.




How to use this implementation:
	$ make
	$ ./exec_GA [parameters]


Useful methods:
	$ GA model = ga_create( ... )		-> creates the population;
	$ fit(model)						-> trains the model;
	$ predict_classification(model, X, No_of_samples)	-> Returns a list with the prediction of the given dataset.
	$ predict_regression(model, X, No_of_samples)   	-> Returns a list with the prediction of the given dataset.


