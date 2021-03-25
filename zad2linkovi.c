#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char* argv[]){
	struct stat sb;

	if (argc < 2){
		fprintf(stderr, "No arguments given.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 1; i < argc; i++){
		if (stat(argv[i], &sb) == -1){
			perror(argv[i]);
			continue;
		}
		printf("File %s has %d links linking to it.\n", argv[i], sb.st_nlink);

	}
	return 0;
		
}

