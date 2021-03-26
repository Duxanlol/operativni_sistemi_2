#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(){
	printf("Sadrzaj trenutnog direktorijuma:\n");
	if (execlp("ls", "ls", "-Aog", (char *) NULL) != 0){
		int e = errno;
		printf(strerror(e));
		printf("\n");
	}		
	printf("kraj ispisa. \n");
	return 0;
}
