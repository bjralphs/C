/**************************
 * A program that uses the fork() and exec() system calls, 
 * demonstrating understanding of their outputs and correct usage.
 * ***********************/

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	//CREATE CHILD PROCESS BY CALLING FORK()
	pid_t pid = fork();

	if (pid == 0) { //CHILD PROCESS
		printf("CHILD started. \n");
		if (argc == 1) {//IF NO INPUT RECIEVED
			printf("No arguments provided. Terminating child. \n");
		}
		else if (argc == 2) {//IF ONLY ONE INPUT RECIEVED
			printf("One argument provided. Calling execlp(), never to return (sniff) \n");
			execlp(argv[1], argv[1], NULL);
		}
		else {//IF MORE THAN ONE INPUT RECIEVED
			printf("More than one argument provided. Calling execvp(), never to return (sniff) \n");
			execvp(argv[1], &argv[1]);
		}
	}
	else { //PARENT PROCESS STATEMENT
		printf("PARENT started, now waiting for process ID#%d\n", pid);
		int status;
		waitpid(pid, &status, 0); //WAIT FOR CHILD TERMINATION
		printf("PARENT resumed. Child exit code of %d. Now terminating parent\n", WEXITSTATUS(status));
	}

	return 0;

	}
