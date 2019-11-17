/* test_char_driver.c
** Team: 
** Version: 1.0
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	int fd, i,num;
	ssize_t ret;
	char my_buf[12]="16";
	
	fd = open( "/dev/misc_char_cdrv", O_RDWR );
	if(fd<0)
		printf("failed acquiring file descriptor return status %d\n",fd);
	
else
{
check_list:
	num=atoi(my_buf);
	if(num >= 0 && num <= 15)
	{
	printf("\n Data is fine:%d",num);
	ret = write( fd, my_buf, sizeof(my_buf));
	close(fd);
	}
	else
	{
	printf("\n Enter your data:");
	scanf("%s",my_buf);
	goto check_list;
	}
	/* write the contents of my buffer into the device */
	//ret = write( fd, my_buf, 2 );
	
	//if(ret<0)
	//	printf("write  operation failed with return status %d\n",ret);
	//close(fd);
}
return 0;
}
