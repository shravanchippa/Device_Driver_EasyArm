#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define lcd_cmd 0x01
#define lcd_data 0x02

int main(int argc, char **argv)
{

	int fd;
	
	fd = open("/dev/lcd2x16", 0);
	
	if (fd < 0) {
		perror("open device leds");
		exit(1);
	}
	ioctl(fd,lcd_data, 'A');
	ioctl(fd,lcd_data, 'B');
	ioctl(fd,lcd_cmd, 0xc0);
	ioctl(fd,lcd_data, 'C');
	ioctl(fd,lcd_data, 'D');
	close(fd);
	return 0;
}

