#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
	printf("Sadrzaj trenutnog direktorijuma:\n");
	if (fork() == 0){
		if (execlp("ls", "ls", "-Aog", (char *) NULL) != 0){
			int e = errno;
			printf(strerror(e));
			printf("\n");
		}		
	}
	else{
		wait(NULL);
		printf("kraj ispisa. \n");
	}
	return 0;
}
