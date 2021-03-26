#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int min_size = 100;
char cwd[PATH_MAX];

int filedetail(char *dir){
	struct stat sb;
	char path[PATH_MAX];
	strcpy(path, cwd);
	strcat(path, dir);

		if(stat(path, &sb) == -1){
			perror(path);
		}else{
			if( min_size <= (sb.st_size)/1000)
			printf("%-25s\t %3d %4d KB\n", dir, sb.st_nlink, (sb.st_size)/1000);
		}
}
	
int listdir(char *dir){
	struct dirent *dp;
	DIR *dirp;
	dirp = opendir(dir);
	if (dirp == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	while((dp=readdir(dirp))!= NULL){
		if(dp->d_type == 8)
			filedetail(dp->d_name);
	}
}


int main(int argc, char **argv){
	if(argc == 1){
		printf("Incorrect usage, provide a directory to list.\n");
	}else{
		if (argc < 4){
			if(argc == 3){
				char *rest;
				int minfilter;
				errno = 0;
				minfilter = strtol(argv[2], &rest, 10);
				if (errno != 0){
					printf("ERRNO HIT\n");
					perror("strtol");	
					exit(EXIT_FAILURE);
				}
				if(rest == argv[2]){
					printf("Second argument couldn't be parsed, defaulting to 100KB\n");
				}else{
					min_size = minfilter;
				}

			}
			strcpy(cwd, argv[1]);
			if ((strcmp(cwd, ".") == 0) || strcmp(cwd, "..") == 0)
				strcat(cwd,"/");
			listdir(argv[1]);
		}
	}
	return 0;
}
