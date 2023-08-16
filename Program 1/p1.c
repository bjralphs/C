/**************************
 * Prints a program description, 
 * then displays the array contents using a loop, 
 * and finally shows the total number of printed strings.
 * ***********************/

#include <stdio.h>

int main(int argc, char *argv[]){
	printf("Bryan Ralphs, M1: Program Prints Content Of An Array \n");
	int count = 0;
	for (int i = 0; i < argc; i++) {
		printf("Argument %d: %s\n", i, argv[i]);
		count += 1;
	}
	printf("Number of strings printed: %d\n", count);

	return 0;
}
