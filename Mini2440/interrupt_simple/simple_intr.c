/***************************************************************************
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/irq.h>
#include <asm/irq.h>

int md;
int enter;

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
    	printk("inside the handler\n");
	printk("\nentered=%d \n",enter++);

	return IRQ_RETVAL(IRQ_HANDLED);
}

int init_module(void)
{
	printk("\nsimple modules loaded\n");
	request_irq(IRQ_EINT8, buttons_interrupt, IRQ_TYPE_EDGE_BOTH, "button irqs", &md);	
	return 0;
}

void cleanup_module(void)
{
	printk("\nsimple module unloaded\n");
	free_irq(IRQ_EINT8, &md);			
}

MODULE_LICENSE("GPL");

