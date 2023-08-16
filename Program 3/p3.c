/**************************
 * Creates a multithreaded program to factorize command line numeric inputs,
 * with each child thread handling a number and returning its prime factors, 
 * while the parent thread waits and then displays the results.
 * ***********************/


#include <stdlib.h>
#include <pthread.h> //POSIX
#include <math.h>
#include <stdio.h>

// STRUCTURE FOR SHARING DATA WITH EACH THREAD
typedef struct {
	int num;
	int* variables;
	int variable_count;
} PrimeData;

//LEGIT FUNCTION FOR CALCULATING PRIME
void* calc_components(void* arg) {
	PrimeData* data = (PrimeData*) arg;
	int number = data->num;
	data->variable_count = 0;


	//USES SQRT TO FIND PRIME
	for (int x = 2; x <= sqrt(number); x++) {
		while (number % x == 0) {
			data->variables[data->variable_count++] = x;
			number = number / x;
		}
	}

	if (number > 1){
	       	data->variables[data->variable_count++] = number;
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	//TRIAL DIVISION METHOD FOR FACTORING
	if (argc < 2) {
		printf("Usage: %s <number to factor>...\n", argv[0]);
		return 1;
	}
	pthread_t threads[argc - 1];
	PrimeData data[argc - 1];

	//CREATE THREAD AND CALC PRIME
	for (int y = 0; y < argc - 1; y++) {
		data[y].num = atoi(argv[y + 1]);
		data[y].variables = malloc(sizeof(int) * data[y].num);
		pthread_create(&threads[y], NULL, calc_components, &data[y]);
	}

	//WAIT FOR THREAD COMPLETION
	for (int z = 0; z < argc - 1; z++) {
		pthread_join(threads[z], NULL);
		printf("%d: ", data[z].num);
		for (int w = 0; w < data[z].variable_count; w++) {
			printf("%d ", data[z].variables[w]); 
		}
		printf("\n");

		// RELEASE THE MEMORY!
		free(data[z].variables);

	}

	return 0;
}


