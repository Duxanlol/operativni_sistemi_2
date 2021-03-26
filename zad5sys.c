#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
	printf("Sadrzaj trenutnog direktorijuma:\n");
	system("ls -Aog");	
	printf("kraj ispisa. \n");
	return 0;
}
