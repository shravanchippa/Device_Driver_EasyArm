
/***************************************************************************
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/irq.h>
#include <asm/irq.h>


struct tasklet_struct task;
int id,i,tasklet_num,handler_num;
int md;
int enter;

void my_func(unsigned long);
void my_func(unsigned long data)
{
	printk(" tasklet number %d\n",tasklet_num++);
	for(i=0;i<200;i++);
	printk("tasklet is scheduled\n");
	
}

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
    	printk("inside the handler\n");
	printk("\nentered=%d \n",enter++);
	tasklet_schedule(&task);
	return IRQ_RETVAL(IRQ_HANDLED);
}

int init_module(void)

{	task.func=&my_func;
	printk("simple modules loaded\n");
	request_irq(IRQ_EINT8, buttons_interrupt, IRQ_TYPE_EDGE_BOTH, "button irqs", &md);	
	return 0;
}

void cleanup_module(void)
{
	printk(" simple module unloaded\n");
	free_irq(IRQ_EINT8, &md);			
}

MODULE_LICENSE("GPL");
