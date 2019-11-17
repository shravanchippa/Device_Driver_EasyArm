
#include"lcd.h"

static unsigned long lcd_table [7] = {S3C2410_GPF0,	S3C2410_GPF1,	S3C2410_GPF2,	S3C2410_GPF3,S3C2410_GPF4,S3C2410_GPF5,S3C2410_GPG0 };
static unsigned int lcd_cfg_table [7] = {S3C2410_GPF0_OUTP,S3C2410_GPF1_OUTP,S3C2410_GPF2_OUTP,	S3C2410_GPF3_OUTP,S3C2410_GPF4_OUTP,S3C2410_GPF5_OUTP,S3C2410_GPG0_OUTP};

static int sbc2440_lcd_ioctl(	struct inode *inode, 	struct file *file, 	unsigned int cmd, 	unsigned long arg)
{
	switch(cmd) {
	case lcd_cmd:
				SendCommand(arg);
				return 0;
	case lcd_data:
				SendData(arg);
				return 0;
	default:
		return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	sbc2440_lcd_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};


static void SendNibble(unsigned char value)
{
    value &= 0x0f;                 // mask off high nibble

    if(value & 0x01)
	s3c2410_gpio_setpin(D0,1);
	else
	s3c2410_gpio_setpin(D0,0);

	if(value & 0x02)
	s3c2410_gpio_setpin(D1,1);
	else
	s3c2410_gpio_setpin(D1,0);

	if(value & 0x04)
	s3c2410_gpio_setpin(D2,1);
	else
	s3c2410_gpio_setpin(D2,0);

	if(value & 0x08)
	s3c2410_gpio_setpin(D3,1);
	else
	s3c2410_gpio_setpin(D3,0);
	
	s3c2410_gpio_setpin(RW,0); //write
	s3c2410_gpio_setpin(EN,1); //enable
	mdelay(1);
	s3c2410_gpio_setpin(EN,0); //disable
}

void SendCommand(unsigned char value)
{
    s3c2410_gpio_setpin(RS,1);        //command
    SendNibble(value>>4);             // send high nibble
    SendNibble(value);                // send low nibble
}

void SendData(unsigned char value)
{
    s3c2410_gpio_setpin(RS,0);        //Data
    SendNibble(value>>4);             // send high nibble
    SendNibble(value);                // send low nibble
}


static int __init dev_init(void)
{
	int ret;

	int i;
	
	for (i = 0; i < 7; i++) {
		s3c2410_gpio_cfgpin(lcd_table[i], lcd_cfg_table[i]);
		s3c2410_gpio_setpin(lcd_table[i], 0);
	}
	ret = misc_register(&misc);
	printk (DEVICE_NAME"\tinitialized\n");

	mdelay(50);
	s3c2410_gpio_setpin(D0,1);
	s3c2410_gpio_setpin(D1,1);
    	mdelay(5);
    	s3c2410_gpio_setpin(D0,1);
	s3c2410_gpio_setpin(D1,1);
   	 mdelay(5);
    	s3c2410_gpio_setpin(D0,1);
	s3c2410_gpio_setpin(D1,1);
    	mdelay(5);
    	s3c2410_gpio_setpin(D0,0);
	s3c2410_gpio_setpin(D1,1);
    	mdelay(5);

	// The chip is now expecting 4-bit communication. The next
    // commands configure the display for 2 lines of 16 chars.
    //
    SendCommand(0x28);
    SendCommand(0x08);
    SendCommand(0x0c);
    SendCommand(0x01);
    SendCommand(0x06);

	printk (DEVICE_NAME"\tinitialized\n");
	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyARM Inc.");
