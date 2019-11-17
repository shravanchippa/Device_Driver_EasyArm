#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main()
{
int fd,return_len;
char chbuf[128]="linux char device driver training example";


	fd=open("/dev/sample_cdrv",O_RDWR);
	if(fd < 0){
		printf("\nUnable to open file\n");
		return 0;
	}
	else
	{
		printf("\nfile is opened successfully with fd = %d\n",fd);
		return_len = write(fd,&chbuf,strlen(chbuf));
		printf("\n Successfully write %s with length = %d\n",chbuf,return_len);
		close(fd);
		return 0;
	}
}



