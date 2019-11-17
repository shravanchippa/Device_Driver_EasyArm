#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void delay (int aa)
{
int i,j;
for(i=0; i<aa; i++)
{
for (j=0; j<100000; j++)
{}
}
}

int main(void)
{

	int fd,cnt;
	char *buf="ON";
	char buf2[4];
	fd=open("/dev/MYLED",0x666);
	if(fd==-1)
	{
	 printf("error\n");
	exit(1);
	}
        while(1)
	{
		buf="OFF";
		cnt=write(fd,buf,4);                //writing in kernel space
        if(cnt==-1) 
        {
        	printf("error\n");
        	exit(1);
		}
	
		delay(100);
		buf="ON";
	
		cnt=write(fd,buf,4);                //writing in kernel space
		if(cnt==-1) 
		{
        	printf("error\n");
        	exit(1);
        }
	
		delay(100);

	}
		cnt= read(fd,buf2,4);		//reading from kernel space
		printf("Reading from Kernel : %s\n",buf2);
		if(cnt==-1)
		{
        	printf("error\n");
        	exit(1);
        }
	

	close(fd);
	return 0;
}
