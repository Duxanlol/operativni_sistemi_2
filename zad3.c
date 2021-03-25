#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
int filedetail(char *dir, char *d_name){
	struct stat sb;
	char fullpath[255];
	strcpy(fullpath,dir);
	if (d_name[0] == '.')
		return 0;
	if (dir[strlen(dir)]!='/')
		strcat(fullpath,"/");
	strcat(fullpath,d_name);
	if (stat(fullpath, &sb) == -1){
		perror(d_name);
	}else{
		char perm[10] = {"----------"};
		if (S_ISDIR(sb.st_mode))
			perm[0] = 'd';
		char permchar[3] = {"xwr"};
		int permbits = sb.st_mode;
		for (int i = 0; i < 3; i++){
			for (int permind = 0; permind < 3; permind++){
				if (permbits & 1){
					perm[9 - permind - 3*i] = permchar[permind];
				}
				permbits >>= 1;
			}		
		}
		perm[10] = '\0';
		int linkcnt = sb.st_nlink;
		int blksize = sb.st_size;
		struct tm *tm = localtime(&sb.st_ctime);
		printf("%s %2d %5d %d-%d-%d %02d:%02d %s\n",
			perm, linkcnt, blksize, tm->tm_year+1900, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, d_name);
	}
	return 0;
}
int dirdetail(char *dir){
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
				fflush(stdout);
				filedetail(dir, dp->d_name);
			}
		}
	closedir(dirp);
	return 0;
}
int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr,"No path provided. \n");
		exit(EXIT_FAILURE);
	}
	for(int i = 1; i < argc; i++){
		printf("%s\n",argv[i]);
		dirdetail(argv[i]);
	}			
	exit(EXIT_SUCCESS);
}
