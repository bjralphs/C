/**************************
 * The program computes the Page Index and Offset 
 * from a given hexadecimal logical address with a 4KB page size.
 * ***********************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	//IN THE CASE AN INVALID ARGUMENT IS PROVIDED
	if(argc != 2) {
        	printf("You did not provide a valid argument.\n");
        	return 1;
	}

	// CONVERT INPUT HEX TO LONG INT
	char *ptr;
	long int logical_address = strtol(argv[1], &ptr, 16);


	//CALCULATE THE PAGE OFFSET AND INDEX
	long int index = logical_address >> 12;
	long int offset = logical_address & 0xFFF;

	// OUTPUT
	printf("Logical Addr:0x%08lX - Page Index:0x%08lX - Offset:0x%08lX\n", logical_address, index, offset);

	return 0;
}
