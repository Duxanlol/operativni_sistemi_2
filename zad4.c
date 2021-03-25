#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

int opt_mesec[13];
int opt_sort = 0;
int opt_mesecind = 0;
char opt_char = 0;
int opt_year = 0;
char cwd[255];
int filter(const struct dirent *);
int filterchar (const char* d_name, char f);
int filtertime (const char *dir, const char* d_name);
int filtermesec(const int mesec, int opt_mesecind);
int filteryear(const int year, int opt_year);

int filterchar (const char* d_name, char f){
	return (d_name[0]==f);
}
int filtertime(const char *dir, const char* d_name){
	int allow = 1;
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
		return 0;
	}else{
		struct tm *tm = localtime(&sb.st_ctime);
		if (opt_year)
			allow &= filteryear(tm->tm_year+1900, opt_year);
		if (opt_mesecind){
			allow &= filtermesec(tm->tm_mon, opt_mesecind);
		}	
	}
	return allow;
}

int filtermesec(int mesec, int opt_mesecind){
	return (mesec+1==opt_mesecind);
}
int filteryear(int year, int opt_year){
	return (year > opt_year);
}

int filter(const struct dirent *dirent){
	int allow = 1;
	if (opt_char)
		allow &= filterchar(dirent->d_name, opt_char);
	if (opt_year || opt_mesecind)
		allow &= filtertime(cwd, dirent->d_name);
	return allow;	
}

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
int listdir(char *dir){
	struct dirent **namelist;
	int entrycnt = 0;
	if (opt_sort)
		entrycnt = scandir(dir, &namelist, filter, alphasort);
	else	
		entrycnt = scandir(dir, &namelist, filter, NULL);

		if (entrycnt == -1){
			int e = errno;
			printf("%s %s\n",dir, strerror(e));
			return -1;
		}
		for (int entryind = 0; entryind < entrycnt; entryind++){
			filedetail(cwd, namelist[entryind]->d_name);
			free(namelist[entryind]);
		}
	free(namelist);
	return 0;
}
int main(int argc, char **argv){
	int opt;
	struct dirent *dp;
	DIR *dirp;

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"januar", no_argument, &opt_mesec[1], 1},
			{"februar", no_argument, &opt_mesec[2], 1},
			{"mart", no_argument, &opt_mesec[3], 1},
			{"april", no_argument, &opt_mesec[4], 1},
			{"maj", no_argument, &opt_mesec[5], 1},
			{"jun", no_argument, &opt_mesec[6], 1},
			{"jul", no_argument, &opt_mesec[7], 1},
			{"avgust", no_argument, &opt_mesec[8], 1},
			{"septembar", no_argument, &opt_mesec[9], 1},
			{"oktobar", no_argument, &opt_mesec[10], 1},
			{"novembar", no_argument, &opt_mesec[11], 1},
			{"decembat", no_argument, &opt_mesec[12], 1},
			{"sort", no_argument, &opt_sort, 1},
			{0,0,0,0}
		};
		
		opt = getopt_long(argc, argv, "s:p:", long_options, &option_index);	
		if (opt == -1)
			break;

		switch(opt){
			case 0:
				break;
			case 's':
				opt_char = optarg[0];
				break;
			case 'p':
				opt_year = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s [-AR] [directory ...]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	if (optind >= argc){
		printf("No path(s) provided. \n");
		exit(EXIT_FAILURE);
	}
	for(int i = 1; i < 13; i++){
		if (opt_mesec[i]){
			if(!opt_mesecind)
				opt_mesecind = i;
			else{
				printf("There can be only one month option. \n");
				exit(EXIT_FAILURE);
			}
		}
	}
	for(int i = optind; i < argc; i++){
		printf("%s\n",argv[i]);
		strcpy(cwd, argv[i]);
		listdir(argv[i]);
	}			
	exit(EXIT_SUCCESS);
}
