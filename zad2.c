#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int recursive = 0;
int all = 0;
void printentry(char *d_name, int d_type, int depth){
	/*
	char padding[20];
	memset(padding, ' ', depth);
	printf(padding);
	*/
	for(int i = 0; i < depth*2; i++){
		printf(" ");
	}
	switch(d_type)
	{
		case DT_DIR:
			printf("+%s\n",d_name);
			break;
		case DT_REG:
			printf("-%s\n",d_name);
			break;
	}
	fflush(stdout);
}
int listdir(char *dir, int depth){
	struct dirent *dp;
	DIR *dirp;
	dirp = opendir(dir);
		if (dirp == NULL){
			int e = errno;
			printf("%s %s\n",dir, strerror(e));
			return -1;
		}
		while ((dp=readdir(dirp))!= NULL){
			if ((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name,"..") != 0)){
				if(!(all == 0 && dp->d_name[0] == '.'))
				printentry(dp->d_name, dp->d_type, depth);
			if (recursive == 1 && dp->d_type == DT_DIR){
				char newPath[255] = "";
				fflush(stdout);
				strcpy(newPath, dir);
				strcat(newPath, "/");
				strcat(newPath, dp->d_name);
				listdir(newPath, depth + 1);
			}
			fflush(stdout);
			}
		}
	closedir(dirp);
	return 0;
}
int main(int argc, char **argv){
	int opt;
	struct dirent *dp;
	DIR *dirp;
	while((opt = getopt(argc,argv,"AR")) != -1){
		switch(opt){
			case 'A':
				all = 1;
				break;
			case 'R':
				recursive = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-AR] [directory ...]\n", argv[0]);
				exit(-1);
		}
	}
	//printf("arg recursive is %d \n", recursive);
	//printf("arg all is %d \n", all);
	//printf("optind = %d", optind);
	if (optind >= argc){
		printf("No path provided. \n");
		return -1;
	}
	for(int i = optind; i < argc; i++){
		printf("%s\n",argv[i]);
		listdir(argv[i], 1);
	}			
	return 0;
}
