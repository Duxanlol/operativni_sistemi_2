#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
/*
 *
 *	OVO RADI ONO STO PISE U ZADATKU (ALI TO STO PISE U ZADATKU NE RADI
 *	BAR SE MENI TAKO CINI). nesto kod permisija puca, ako promenim da je sve 777 kopira bez
 *	problema.
 *
 *
 */
int make_directory_set_rights(char *path){
	printf("new directory is %s \n", path);
	if (mkdir(path, 0570) && errno != EEXIST){
		perror(path);
		return -1;
	}
	chmod(path, 0570); //Ovo mora zbog umask-a; (mkdir ne postavi rights kako treba);
	return 0;
}
int file_cut(char *origin, char *dest, char *file){
	char oldpath[PATH_MAX];
	char newpath[PATH_MAX];
	strcpy(oldpath,origin);
	strcat(oldpath, file);
	strcpy(newpath, dest);
	strcat(newpath, file);
	if (rename(oldpath, newpath) == -1)
		perror(file);

	chmod(newpath, 0570);
}
int main(int argc, char **argv){
	if(argc < 4){
		printf("Invalid usage\n");
		exit(EXIT_FAILURE);
	}else{
		if(make_directory_set_rights(argv[2]) != 0){
			printf("Couldn't make directory \n");
		}
		DIR* dir = opendir(argv[1]);
		if (dir){
			for(int i = 3; i < argc; i++){
				file_cut(argv[1], argv[2], argv[i]);
			}	
			closedir(dir);
		}else{
			perror(argv[1]);
		}

	}
	return 0;
}
