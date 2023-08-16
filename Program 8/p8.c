/**************************
 * A program that opens a directory,
 * displays each file's size and name, 
 * and recursively accesses subdirectories; 
 * it defaults to the current directory or takes a specified one, 
 * using certain programming APIs.
 * ***********************/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

//Initialize Global Variable To Track Total Size
unsigned long long fileSize = 0;

void listDirectory(char *baseDirectory, int indentation){
    	DIR *directory; //Directory Stream
	struct dirent *entry; //Directory Entry
    	struct stat fileInfo;
    	char *path;

    	if(!(directory = opendir(baseDirectory)))
        	return;

    	while ((entry = readdir(directory)) != NULL) {
        	asprintf(&path, "%s/%s", baseDirectory, entry->d_name);
        	lstat(path, &fileInfo);
		// Check If The Item Is A Directory
        	if(S_ISDIR(fileInfo.st_mode)) {
			// If The Directory Is Current Or Parent Directory, Skip
            		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                		continue;
			// Print Directory Name With Indentation
            		printf("%*s[directory %s]\n", indentation, "", entry->d_name);
			// Recursively List Content Of This Directory
            		listDirectory(path, indentation + 4);
        	} else if(S_ISREG(fileInfo.st_mode)) {
			// If It's A Regular File, Print Its Size And Name With Indentation
            		printf("%*s%lld:%s\n", indentation, "", fileInfo.st_size, entry->d_name);
			// Add The File Size To The Global Variable
            		fileSize += fileInfo.st_size;
        	}
		// Free The Allocated Memory For Path
        	free(path);
    	}
	// Close The Directory Stream
    	closedir(directory);
}

int main(int argc, char *argv[]) {
	// Default Directory To Open Is Current Directory
    	char *targetDirectory = ".";
	// If There's An Argument Provided, Use It As The Directory To Open
    	if(argc > 1) {
        	targetDirectory = argv[1];
    	}
    	
	// List The Content Of The Directory
    	listDirectory(targetDirectory, 0);
	// Print The Total File Space Used
    	printf("\nTotal file space used: %llu\n", fileSize);
    	return 0;
}
