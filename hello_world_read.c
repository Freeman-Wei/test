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

	char buf[20]; 

	testdev = open("/dev/hello_world",O_RDWR| O_NONBLOCK); 

	if ( testdev == -1 ) 

	{ 

		printf("Cann't open file \n"); 
		perror("error is ");

		return -1; 

	} 
	memset(buf,0,sizeof(buf));

	read(testdev,buf,20); 

	printf("%s\n",buf); 

	close(testdev); 

	return 0;

} 