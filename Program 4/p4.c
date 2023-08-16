/**************************
 * The program reads process start times and durations,
 * then evaluates them using four CPU scheduling algorithms, 
 * displaying average response, wait, and turnaround times for each; 
 * it uses a 100ms time quantum for Round Robin and handles up to 100 processes.
 ************************/


#include <stdio.h>
#include <stdlib.h>

#define N 20 //number of processes
#define QUANTUM 100 //Time quantum

//ALGORITHMS
void FCFS(int arrival_time[], int burst_time[]);
void SJF(int arrival_time[], int burst_time[]);
void SRTF(int arrival_time[], int burst_time[]);
void RR(int arrival_time[], int burst_time[]);

int main()
{
	int arrival_time[N];
	int burst_time[N];

	int i = 0;

	while(scanf("%d %d", &arrival_time[i], &burst_time[i]) == 2 && i < N)
	{
		i++;
	}
	
	//call scheduling algorithms
	printf("\nFirst Come, First Served: \n");
	FCFS(arrival_time, burst_time);

	printf("\nShortest Job First: \n");
	SJF(arrival_time, burst_time);

	printf("\nShortest Remaining Time First: \n");
	SRTF(arrival_time, burst_time);

	printf("\nRound Robin with Time Quantum of 100:");
	RR(arrival_time, burst_time);

	return 0;
}
//FIRST COME, FIRST SERVED SCHEDULING ALGORITHM
void FCFS(int arrival_time[], int burst_time[])
{
	//Declaring arrays to store waiting time, turnaround time, and resposne time
	int waiting_time[N], turn_around_time[N], response_time[N];
	//Initializing variables to store totals
	int total_wt = 0, total_tat = 0, total_rt = 0;
	//Variable for keeping track of completion time of processes
	int completion_time = 0;


	// waiting time for the first process will be 0
	waiting_time[0] = 0;
	response_time[0] = 0;
        completion_time = burst_time[0];
	

	// calculating turnaround time by adding burst_time[i] + waiting_time[i]
    	for (int i = 0; i < N; i++)
    	{
		if (arrival_time[i] > completion_time) {
			waiting_time[i] = 0;
			completion_time = arrival_time[i] + burst_time[i];
		} else {
			waiting_time[i] = completion_time - arrival_time[i];
			completion_time += burst_time[i];
		}

        	response_time[i] = waiting_time[i];
		turn_around_time[i] = waiting_time[i] + burst_time[i];

		total_wt += waiting_time[i];
		total_tat += turn_around_time[i];
        	total_rt += response_time[i];
    	}

	//print results
    	printf("Avg. Resp.:%.2f, Avg. T.A.:%.2f, Avg. Wait:%.2f\n",
           	(float)total_rt / N,
           	(float)total_tat / N,
           	(float)total_wt / N);
}
//SHORTEST JOB FIRST SCHEDULING ALGORITHM
void SJF(int arrival_time[], int burst_time[])
{
	//Arrays for waiting time and turnaround time
    	int waiting_time[N], turn_around_time[N], total_wt = 0, total_tat = 0;
    	int response_time[N], total_rt = 0;
    	int n = N;
	//copy burst time into array
    	int completed[N];
    	for(int i = 0; i < n; i++) {
        	completed[i] = 0;
    	}
	
	int current_time = 0;
	while(1){
		int idx = -1; //index of shortest job
		int mn = 10000000;
		for(int i = 0; i < n; i++) {
			if(arrival_time[i] <= current_time && completed[i] == 0) {
				if(burst_time[i] < mn) {
					mn = burst_time[i];
					idx = i;
				}
				if(burst_time[i] == mn) {
					if(arrival_time[i] < arrival_time[idx]) { //break tie based on arrival time
						mn = burst_time[i];
						idx = i;
					}
				}
			}
		}

		if(idx != -1) { //if process if found
			waiting_time[idx] = current_time - arrival_time[idx]; // Calculate waiting time
			turn_around_time[idx] = waiting_time[idx] + burst_time[idx]; // Calculate turnarouund time
			response_time[idx] = waiting_time[idx];// Calculate response time
			total_wt += waiting_time[idx]; //Update total waiting time
			total_tat += turn_around_time[idx]; //Update total turnaround time
			total_rt += response_time[idx]; //
			completed[idx] = 1; // Mark the process as complete
			current_time += burst_time[idx]; //Update the current time
		} else {
			current_time++;
		       	//Increment current time if no process is found
		}

		int done = 1;
	       	//Flag to check if all processes are completed
		for(int i = 0; i < n; i++) {
			if(completed[i] == 0) {
				done = 0;
			       	// There is at least one process remaiing
				break;
			}
		}
		if(done) {
			break;
		       	// Exit the loop if all processes are completed
		}
	}

    	// Print results
    	printf("Avg. Resp.:%.2f, Avg. T.A.:%.2f, Avg. Wait:%.2f\n", (float)total_rt / n, (float)total_tat / n, (float)total_wt / n);
}
//SHORTEST REMAINING TIME FIRST SCHEDULING ALGORITHM
void SRTF(int arrival_time[], int burst_time[])
{
	//Initialize total waiting time, total turnaround, and response time
    	int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
	//Initialize arrays to store the remaiing burst time, completion time, turnaround time, waiting time
	//and response time
    	int remaining_burst[N], completion_time[N], turnaround_time[N], waiting_time[N], response_time[N];
	//Initialize variables 
    	int completed = 0, current_time = 0, prev = 0;
	//Initialize remaining burst time for each process
    	for (int i = 0; i < N; i++){
        	remaining_burst[i] = burst_time[i];
	}
	//Loop until all processes are completed
    	while (completed != N) 
	{
		//Variables to hold the index of the process
        	int idx = -1;
        	int mn = 10000000;
		int found = 0;
		// Loop through all processes
        	for (int i = 0; i < N; i++) 
		{
            		if (arrival_time[i] <= current_time && remaining_burst[i] < mn && remaining_burst[i] > 0)
	       		{
                		mn = remaining_burst[i];
                		idx = i;
				found = 1;
            		}
        	}
		// If a process was found, update its remaining burst time and compute remaining variables
		if (found == 1)
	       	{
            		if (remaining_burst[idx] == burst_time[idx])
                		response_time[idx] = current_time - arrival_time[idx];

            		remaining_burst[idx] -= 1;
            		current_time++;
            		prev = current_time;
			//If the process has finished, compute its completion time and remaining variables
            		if (remaining_burst[idx] == 0)
		       	{	
                		completion_time[idx] = current_time;
                		turnaround_time[idx] = completion_time[idx] - arrival_time[idx];
                		waiting_time[idx] = turnaround_time[idx] - burst_time[idx];
				//Update total times
                		total_turnaround_time += turnaround_time[idx];
                		total_waiting_time += waiting_time[idx];
                		total_response_time += response_time[idx];
                		completed++;
            		}
        	}
	       	else
		{
			int next_arrival = 10000000;
			for(int i = 0; i < N; i++)
			{
				if(arrival_time[i] > current_time && arrival_time[i] < next_arrival)
				{
					next_arrival = arrival_time[i];
				}
			}
            		current_time = next_arrival;
		}
   	 }	
	//Calculate final totals
    	float avg_response_time = (float)total_response_time / N;
    	float avg_turnaround_time = (float)total_turnaround_time / N;
    	float avg_waiting_time = (float)total_waiting_time / N;

    	printf("Avg. Resp.:%.2f, Avg. T.A.:%.2f, Avg. Wait:%.2f\n", avg_response_time, avg_turnaround_time, avg_waiting_time);
}

void RR(int arrival_time[], int burst_time[])
{
	//Initializing the number of processes
	int n = N;

	//Initializing the arrays that will store waiting time, turnaround time
	//response time, and remaiing time for each process
	int waiting_time[N] = {0};
	int turnaround_time[N] = {0};
	int response_time[N] = {0};
	int remaining_time[N];
	//Initializing a variable to count the number of completed processes
	int completed = 0;
	//Initializing the current time to 0
	int current_time = 0;

	//Initializing the remaining time array with the burst times for each process
	for(int i = 0; i < n; i++)
	{
		remaining_time[i] = burst_time[i];
	}
	//While all processes are not completed
	while (completed != n)
	{
		int done = 0;
		//Loop through through all processes
		for (int i = 0; i < n; i++) {
			//If its the first response for the process, set the response time
			if (arrival_time[i] <= current_time && remaining_time[i] > 0) {
				if (response_time[i] == 0 ) {
					response_time[i] = current_time - arrival_time[i];
				}
				//If remaining time is less or equal to QUANTUM, complete the process
				if (remaining_time[i] <= QUANTUM) {
					current_time += remaining_time[i];
					waiting_time[i] = current_time - arrival_time[i] - burst_time[i];
					turnaround_time[i] = current_time - arrival_time[i];
					remaining_time[i] = 0;
					completed++;
				} else { //If remaining time is more than QUANTUM, process only part of it
					current_time += QUANTUM;
					remaining_time[i] -= QUANTUM;
				}
			}
		}
		//If no process was done in this iteration, increment time
		if (!done) {
			current_time++;
		}
	}
	//Initialize final variables to present
	int total_waiting_time = 0;
	int total_turnaround_time = 0;
	int total_response_time = 0;

	for (int i = 0; i < n; i++) {
		total_response_time += response_time[i];
		total_waiting_time += waiting_time[i];
		total_turnaround_time += turnaround_time[i];
	}

	printf("\nAvg. Resp.:%.2f, Avg. T.A.:%.2f, Avg. Wait:%.2f\n\n",
			(float)total_response_time / n,
			(float)total_turnaround_time / n, 
			(float)total_waiting_time / n);
}
