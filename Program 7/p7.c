/**************************
 * A program that reads disk block access requests, 
 * processes them using four specified algorithms, 
 * and displays the total seek values for each, 
 * starting from the last completed block request, 
 * with a maximum of 100 requests.
 ************************/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

// Defining Maximum Number Of Requests That Can Be Handled
#define MAX_REQUESTS 100

// Function To Compare Two Integers, Used In The QSORT Function
int compare(const void* a, const void* b){
    	return (*(int*)a) - (*(int*)b); // Cast The Void Pointers To INT Pointers And Subtract The Values They Point To
}

// First-Come, First-Serve Algorithm
void FCFS(int requests[], int n, int head){
    	int seek_count = 0;
	// Calculate Total Seek Operation By Iterating Through Each Request
    	for(int y = 0; y < n; y++){
        	seek_count += abs(head - requests[y]);
        	head = requests[y];
    	}
    	printf("FCFS Total Seek: %d\n", seek_count);
}


// Shortest Seek Time First Algorithm
void SSTF(int requests[], int n, int head){
    	int seek_count = 0;
    	int visited[MAX_REQUESTS] = {0};
    	int min, min_index;

    	for(int e = 0; e < n; e++){
        	min = INT_MAX;
        	for(int j = 0; j < n; j++){
			 // If The Request Has Not Been Visited And The Seek Time Is Less Than The Current Minimum
            		if(!visited[j] && abs(head - requests[j]) < min){
                		min = abs(head - requests[j]);
                		min_index = j;
            		}
        	}
        	visited[min_index] = 1;
        	seek_count += abs(head - requests[min_index]);
        	head = requests[min_index];
    	}
    	printf("SSTF Total Seek: %d\n", seek_count);
}

// Look Algorithm
void LOOK(int requests[], int n, int head){
    	int seek_count = 0;
	// Sort The Requests In Ascending Order
    	qsort(requests, n, sizeof(int), compare);
    	int index = 0;
	// Find The First Request That Is Greater Or Equal To The Head
    	for(index = 0; index < n; index++){
        	if(requests[index] >= head){
            		break;
        	}
    	}
	// Calculate Seek Count For Requests Greater Or Equal To Head
    	for(int i = index; i < n; i++){
        	seek_count += abs(requests[i] - head);
        	head = requests[i];
    	}
	// Calculate Seek Count For Requests Less Than The Head
    	for(int i = index - 1; i >= 0; i--){
        	seek_count += abs(requests[i] - head);
        	head = requests[i];
    	}
    	printf("LOOK Total Seek: %d\n", seek_count);
}


// Circular Look Algorithm
void CLOOK(int requests[], int n, int head){
    	int seek_count = 0;
	// Sort The Requests In Ascending Order
    	qsort(requests, n, sizeof(int), compare);

    	int index = 0;
	// Find The First Request That Is Greater Or Equal To The Head
    	for(index = 0; index < n; index++){
        	if(requests[index] >= head){
            		break;
        	}
    	}
	// Calculate Seek Count For Requests Greater Or Equal To Head
    	for(int i = index; i < n; i++){
        	seek_count += abs(requests[i] - head);
        	head = requests[i];
    	}
	// Calculate Seek Count From The Last Request To The First Request
    	seek_count += abs(requests[n-1] - requests[0]);
	// Calculate Seek Count For Requests Less Than The Head
    	for(int s = 0; s < index; s++){
        	seek_count += abs(requests[s] - head);
        	head = requests[s];
    	}
    	printf("C-LOOK Total Seek: %d\n", seek_count);
}


int main(){
	// Initialize Variables
    	int requests[MAX_REQUESTS], x, head;

    	printf("Assignment 7: Block Access Algorithm\n");
    	printf("By: Bryan Ralphs\n");

    	scanf("%d", &head); // Get Initial Position
	// Read Input
    	for(x = 0; scanf("%d", &requests[x]) != EOF && x < MAX_REQUESTS - 1; x++);

    	FCFS(requests, x, head);
    	SSTF(requests, x, head);
    	LOOK(requests, x, head);
    	CLOOK(requests, x, head);

    	//printf("BLOCK ACCESS LIST:\n");
    	//printf("%d\n", head);
    	//for(int k = 0; k < n; k++){
        //	printf("%d\n", requests[k]);
    	//make}

    	return 0;
}
