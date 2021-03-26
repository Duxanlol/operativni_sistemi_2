#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void find_real_path(char *in){
	char buffer[PATH_MAX];
	char *path = realpath(in, buffer);
	int depth = 0;
	if (path){
		for(int i = 0 ; i < strlen(buffer); i++)
			if(buffer[i]=='/')
				depth++;
		printf("%s depth=%d\n", buffer, depth);
	}else{
		perror("realpath");
		exit(EXIT_FAILURE);
	}
	free(path);
}
	


int main(int argc, char **argv){
	char buffer[PATH_MAX];
	char *cwd;
	cwd = getcwd(buffer, sizeof(buffer));
	if(argc == 1){
		printf("%s\n", buffer);
	}else{
		for(int i = 1; i < argc; i++){
			find_real_path(argv[i]);
			}
	}
	return 0;
}
