#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h>
#include <unistd.h>
int main(void) 
{ 
	int testdev; 
	int i; 
	char *buf = "off";
	testdev = open("/dev/led_lv_of",O_RDWR); 
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