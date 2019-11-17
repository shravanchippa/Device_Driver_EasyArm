//or 4 bit data we can take gpf0,gpf1,gpf2and gpf3 and for control lines like gpf4,gpf5 and gpf8 or gpg0

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/errno.h>
#include<linux/fs.h>

#define	D0 S3C2410_GPF0
#define	D1 S3C2410_GPF1
#define	D2 S3C2410_GPF2
#define	D3 S3C2410_GPF3

#define	EN S3C2410_GPF4
#define	RW S3C2410_GPF5
#define	RS S3C2410_GPG0

#define DEVICE_NAME "lcd2x16"

#define lcd_cmd 0x01
#define lcd_data 0x02

void SendCommand(unsigned char value);
void SendData(unsigned char value);
	
