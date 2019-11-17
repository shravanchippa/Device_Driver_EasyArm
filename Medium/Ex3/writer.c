
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

/**
* This program uses the device driver sample_chr_driver and opens a file on it.
* It performs the read, write  operation apart from open() and close()
* Hence it checks the drivers functionality
*/
int main()
{
	int fd, i;
	ssize_t ret;
	char my_buf[4000];
	
	/* fill my buffer with *'s */
	for(i=0; i<4000; i++) my_buf[i] = '*';
	
	/* open the device for read/write/lseek */
	printf("[%d] - Opening device sample_chr_driver\n", getpid() );
	fd = open( "/dev/sample_cdrv", O_RDWR );
         if(fd<0)printf("error");
         printf("PID [%d]\n", getpid());
	
	/* write the contents of my buffer into the device */
	ret = write( fd, my_buf, 4000 );
	close(fd);
}
	
