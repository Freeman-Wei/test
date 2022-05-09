#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h>
#include <unistd.h>
int main() 
{ 
	int testdev; 
	int i; 
	char buf[20] = "Hello World!";
	testdev = open("/dev/hello_world",O_RDWR); 
	if ( testdev == -1 ) 
	{ 
		printf("Cann't open file \n"); 
		perror("error is :");
		return -1;
	} 
	write(testdev,buf,strlen(buf)); 
	close(testdev); 
	return 0;
} 