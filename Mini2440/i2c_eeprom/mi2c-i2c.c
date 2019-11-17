/*
 * An example of how to implement an i2c Linux driver as a loadable module with
 * dynamic device registration. 
 *
 * This driver was meant to be included as a module in a bigger driver which
 * is most likely why you would want to use a kernel i2c solution.
 *
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/i2c.h>

#include "mi2c.h" /* for DRIVER_NAME */


#define EEPROM_DEVICE	"eeprom24C08"


#define EEPROM_ADDRESS	      0x50
#define NUM_DEVICES		1


static struct i2c_client *mi2c_i2c_client[NUM_DEVICES];

static struct i2c_board_info mi2c_board_info[NUM_DEVICES] = {
	{
		I2C_BOARD_INFO(EEPROM_DEVICE, EEPROM_ADDRESS),
	}
};

int mi2c_i2c_get_address(unsigned int device_id)
{
	if (device_id >= NUM_DEVICES)
		return -EINVAL;

	if (!mi2c_i2c_client[device_id])
		return -ENODEV;

	return mi2c_i2c_client[device_id]->addr;
}

int mi2c_i2c_write(unsigned int device_id, unsigned char *buf, int count)
{
	if (device_id >= NUM_DEVICES)
		return -EINVAL;

	if (!mi2c_i2c_client[device_id])
		return -ENODEV;

	return i2c_master_send(mi2c_i2c_client[device_id], buf, count);
}

int mi2c_i2c_read(unsigned int device_id, unsigned char *buf, int count)
{
	if (device_id >= NUM_DEVICES)
		return -EINVAL;

	if (!mi2c_i2c_client[device_id])
		return -ENODEV;

	return i2c_master_recv(mi2c_i2c_client[device_id], buf, count);
}

static int __init
mi2c_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk(KERN_INFO "%s driver registered for device at address 0x%02x\n", 
		client->name, client->addr);

	return 0;
}

static int __exit
mi2c_i2c_remove(struct i2c_client *client)
{
	printk(KERN_INFO "removing %s driver for device at address 0x%02x\n", 
		client->name, client->addr);

	return 0;
}

/* our driver handles two types of device */
static const struct i2c_device_id mi2c_id[] = {
	{ EEPROM_DEVICE, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, mi2c_id);


static struct i2c_driver mi2c_i2c_driver = {
	.driver = {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.id_table	= mi2c_id,
	.probe		= mi2c_i2c_probe,
	.remove		= mi2c_i2c_remove,
};

#define I2C_BUS		0
int __init mi2c_init_i2c(void)
{
	int i, ret;

	struct i2c_adapter *adapter;

	/* register our driver */
	ret = i2c_add_driver(&mi2c_i2c_driver);
	if (ret) {
		printk(KERN_ALERT "Error registering i2c driver\n");
		return ret;
	}

	/* add our devices */
	adapter = i2c_get_adapter(I2C_BUS);

	if (!adapter) {
		printk(KERN_ALERT "i2c_get_adapter(%d) failed\n", I2C_BUS);
		return -1;
	}

	for (i = 0; i < NUM_DEVICES; i++) {
		mi2c_i2c_client[i] = i2c_new_device(adapter, 
							&mi2c_board_info[i]); 

			
		if (!mi2c_i2c_client[i]) {
			printk(KERN_ALERT "i2c_new_device failed\n");
			break;
		}
	}

	i2c_put_adapter(adapter);		
	
	return i == NUM_DEVICES ? 0 : -1;
}

void __exit mi2c_cleanup_i2c(void)
{
	int i;

	for (i = 0; i < NUM_DEVICES; i++) {
		if (mi2c_i2c_client[i]) {
			i2c_unregister_device(mi2c_i2c_client[i]);
			mi2c_i2c_client[i] = NULL;
		}
	}

	i2c_del_driver(&mi2c_i2c_driver);
}


