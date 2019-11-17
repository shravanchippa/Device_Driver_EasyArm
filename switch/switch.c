#include<linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <mach/irqs.h>
#include <linux/gpio.h>

//...........................................................................................
int gpio_irq,gpio_irq1;
int ret,ret1;
//...........................................................................................

irqreturn_t gpio_irq_handler(int irq, void *dev) 
{
static unsigned char led_status=0;
printk("module is in irq handler\n");
led_status^=1;
gpio_set_value(62,led_status);
return IRQ_HANDLED;
}
/*...........................................................................................*/

irqreturn_t gpio_irq_handler1(int irq, void *dev) 
{
        static unsigned char led_status=0;
        printk("module is in irq handler1\n");
led_status^=1;
gpio_set_value(63,led_status);
return IRQ_HANDLED;
}
//..........................................................................................

int myled_init(void)
{	
printk("led_module inserted\n");
gpio_direction_output(62,1);		//to configure the pin as output
gpio_set_value(62,0);                   //set low to that led pin
gpio_direction_output(63,1);		//to configure the pin as output
gpio_set_value(63,0);                   //set low to that led pin
//.....................................................................

gpio_irq = gpio_to_irq(103);
gpio_irq1 = gpio_to_irq(104);

//.....................................................................

printk("gpio_irq number is %d\n",gpio_irq);
if (gpio_irq < 0)
{
        printk("gpio_irq failed\n");
}   
//.....................................................................

printk("gpio_irq1 number is %d\n",gpio_irq1);
if (gpio_irq1 < 0)
{
        printk("gpio_irq1 failed\n");
}  
//.....................................................................

if (gpio_request(103, "sw3") == 0)
{
        gpio_direction_input(103);
        printk("gpio sw3 request succeeded\n");
} 
//.....................................................................

if (gpio_request(104, "sw4") == 0)
{
        gpio_direction_input(104);
        printk("gpio sw4 request succeeded\n");
}       
//.....................................................................

ret = request_irq(gpio_irq,gpio_irq_handler, \
                        IRQ_TYPE_EDGE_RISING,"gpio irq handler",0);
printk("request_irq number is %d\n",ret);
if (ret)
printk("gpio interrupt request failed\n");
else
printk("gpio interrupt request succeeded\n");

//.....................................................................

ret1 = request_irq(gpio_irq1,gpio_irq_handler1,IRQ_TYPE_EDGE_RISING,"gpio irq handler1",0);
printk("request_irq1 number is %d\n",ret);
        if (ret1)
                printk("gpio interrupt1 request failed\n");
        else
                printk("gpio interrupt1 request succeeded\n");
return 0;
}


void myled_exit(void)
{
gpio_free(103);                         
free_irq(gpio_irq,0);
gpio_free(104);
free_irq(gpio_irq1,0);
printk("led_module removed\n");
}

module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("ASHUTOSH SINGH)");
MODULE_DESCRIPTION("this is test modules used to test switch");
MODULE_LICENSE("GPL"); 

