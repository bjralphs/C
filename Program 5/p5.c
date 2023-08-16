/**************************
 * A multithreaded program that shares data between threads, 
 * using mutex locks to prevent race conditions and protect shared data.
 * ***********************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Define Constant Variables
#define MAX_FACTORS 100 
#define BUFFER_SIZE 10000 


// Struct To Store Number And Its Corresponding Factors
typedef struct {
	int number; // Number To Be Factored
	int factors[MAX_FACTORS]; // Factors Of Number
	int count; // The Count Of Factors
} FactorData;

// Declaration Of Global Variables
int number_buffer[BUFFER_SIZE];		// Buffer To Store Numbers
FactorData factor_buffer[BUFFER_SIZE];	// Buffer To Store FactorData
int write_index = 0; 			// Index Where Producer Will Write
int read_index = 0;			// Index Where Producer Will Read And Consumer Will Write
int consume_index = 0;			// Index Where Consumer Will Read
int numbers_to_factor = 0;		// Total Numbers To Be Factored
int numbers_factored = 0;		// Total Numbers Factored
int numbers_processed = 0;		// Total Numbers Processed

// Declaration Of Variables For Synchronizing Producer And Consumer Threads
pthread_mutex_t number_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex For Number_Buffer
pthread_mutex_t factor_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex For Factor_Buffer
pthread_cond_t number_cond = PTHREAD_COND_INITIALIZER; // Conditional For Number_Buffer
pthread_cond_t factor_cond = PTHREAD_COND_INITIALIZER; // Conditional For Factor_Buffer

// Producer Thread Function
void* producer(void* arg) {
	for (int i = 0; i < numbers_to_factor; i++) {
		// Lock Mutex To Protect Number_Buffer
        	pthread_mutex_lock(&number_mutex);
		// Wait If The Buffer Is Full
        	while ((write_index + 1) % BUFFER_SIZE == read_index) {
        		pthread_cond_wait(&number_cond, &number_mutex);
        	}
		// Read A Number From Buffer
        	int number = number_buffer[write_index];
		// Update Write Index
        	write_index = (write_index + 1) % BUFFER_SIZE;
		// Unlock The Mutex
        	pthread_mutex_unlock(&number_mutex);

		// Initialize FactorData For The Number
        	FactorData data;
        	data.number = number;
        	data.count = 0;
		
		//Factor Number
        	for (int x = 2; x <= sqrt(number); x++) {
        		while (number % x == 0) {
                		data.factors[data.count++] = x;
                		number /= x;
            		}
        	}
		
		// If There Is A Remainder, It's Prime
        	if (number > 1) {
            		data.factors[data.count++] = number;
        	}
		
		// Lock The Mutex To Protect Factor_Buffer
        	pthread_mutex_lock(&factor_mutex);
		// Wait If The Buffer Is Full
        	while ((read_index + 1) % BUFFER_SIZE == consume_index) {
           		pthread_cond_wait(&factor_cond, &factor_mutex);
        	}
		// Write The FactorData To The Buffer
        	factor_buffer[read_index] = data;
		// Update The Read Index
        	read_index = (read_index + 1) % BUFFER_SIZE;
		//Increment Total Numbers Factored
        	numbers_factored++;
		// Signal The Consumer That A Number Has Been Factored
        	pthread_cond_signal(&factor_cond);
		// Unlock The Mutex
        	pthread_mutex_unlock(&factor_mutex);
    	}
    	return NULL;
}

// Consumer Thread Function
void* consumer(void* arg) {
    	while (numbers_processed < numbers_to_factor) {
		// Lock The Mutex To protect Factor_Buffer
        	pthread_mutex_lock(&factor_mutex);
		// Wait If Buffer Is Empty
        	while (consume_index == read_index) {
            		pthread_cond_wait(&factor_cond, &factor_mutex);
        	}
		// Read FactorData From Buffer
        	FactorData data = factor_buffer[consume_index];
		// Update Index
        	consume_index = (consume_index + 1) % BUFFER_SIZE;
		// Increment Total Numbers Processed
        	numbers_processed++;
		// Signal Producer That Number Has Been Processes
        	pthread_cond_signal(&number_cond);
		//Unlock Mutex
        	pthread_mutex_unlock(&factor_mutex);
		
		//Print Number And It's Corresponding Factors
        	printf("%d: ", data.number);
        	for (int y = 0; y < data.count; y++) {
            		printf("%d ", data.factors[y]);
        	}
        	printf("\n");
    	}
    	//printf("***All %d numbers processed!***\n", numbers_processed);
    	return NULL;
}

// Function To Add Number To Number_Buffer
void add_number(int number) {
    	if (numbers_to_factor < BUFFER_SIZE) {
		// If Buffer Is Not Full, Add Number
        	number_buffer[numbers_to_factor++] = number;
    	} else {
        	printf("***Buffer is full! Unable to add more numbers***\n");
    	}
}

// Main Function
int main(int argc, char *argv[]) {
        // In Case Of No Argument
        if (argc < 2) {
            	printf("Usage: %s <number to factor>...\n", argv[0]);
            	return 1;
        }

        pthread_t producer_thread, consumer_thread;

        for (int w = 1; w < argc; w++) {
            	// Handle BASH Notation
            	if (argv[w][0] == '{') {
                	char* start_string = argv[w] + 1; // Skip '{'
                	char* end_string = strchr(start_string, '.');
                	*end_string = '\0'; // Null Terminate The Start String
                	end_string += 2; // Skip ".."
                	int start = atoi(start_string);
                	int end = atoi(end_string);
                	for (int j = start; j <= end; j++) {
                    		add_number(j);
                	}
            	} else {
                	add_number(atoi(argv[w])); // If Input Not BASH Statement
            	}
        }

        // Create Producer Thread
        pthread_create(&producer_thread, NULL, producer, NULL);
        // Create Consumer Thread
        pthread_create(&consumer_thread, NULL, consumer, NULL);
        // Wait For Producer Thread
        pthread_join(producer_thread, NULL);
        // Wait For Consumer Thread
        pthread_join(consumer_thread, NULL);

        return 0;
}
