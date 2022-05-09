
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
 
 
int main(int argc, char *argv[])
{
 
	int fd;
	int on;
 
	fd = open("/dev/led0", O_RDWR);
 
	if(fd < 0)
	{
		printf("open error\n");
		exit(1);
	}
	
 
	//控制灯的亮灭
	
	while(1)
	{
		on = 0 ;  //亮灯
		write(fd, &on, 4);
		sleep(1);
 
		on = 1;  //灭灯
		write(fd,&on,4);
		sleep(1);
	
	}
 
 
 
	close(fd);
 
	return 0 ;
 
}
